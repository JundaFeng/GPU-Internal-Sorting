//
// Created by jundafeng on 3/7/22.
//
#ifndef TEST_CUDA_GLOBAL_POINTER_FUNCS_H
#define TEST_CUDA_GLOBAL_POINTER_FUNCS_H
#include <cuda.h>
#include <algorithm>
#include <random>

#include <vector>
#include <functional>
#include <iostream>


#define CATCH_CONFIG_CPP11_TO_STRING
#define CATCH_CONFIG_COLOUR_ANSI
#define CATCH_CONFIG_MAIN

#include "common/fmt.hpp"
#include "common/utils.hpp"

namespace DS
{
   using std::size_t;

   // 插入排序，默认升序
   template<typename Object, class Compare=std::less<Object>>
   std::vector<Object> insertionSort(std::vector<Object> arr, const Compare& cmp = Compare())
   {
      size_t n_total = arr.size();
      for(size_t i = 1; i < n_total; ++i)
      {
         Object tmp = std::move(arr[i]);
         size_t j = i;
         for(; j != 0 && cmp(tmp, arr[j - 1]); --j)
            arr[j] = std::move(arr[j - 1]);
         arr[j] =std::move(tmp);
      }
      return arr;
   }

   // 指定排序范围的插入排序
   // 可以作为快速排序的子函数
   template<typename Object, class Compare=std::less<Object>>
   void insertionSort(std::vector<Object>& arr, size_t left, size_t right,
                      const Compare& cmp = Compare())
   {
      for(size_t i = left; i <= right; ++i)
      {
         Object tmp = std::move(arr[i]);
         int j = i;
         for(; j != 0 && cmp(tmp, arr[j - 1]); --j)
            arr[j] = std::move(arr[j - 1]);
         arr[j] = std::move(tmp);
      }
   }

   template<typename RandomIterator, class Compare>
   void insertionSort(const RandomIterator& begin, const RandomIterator& end,
                      const Compare& cmp)
   {
      if(begin == end)
         return;
      RandomIterator iter;
      for(RandomIterator p = begin + 1; p != end; ++p)
      {
         auto tmp = std::move(*p);
         for(iter = p; iter != begin && cmp(tmp, *(iter - 1)); --iter)
            *iter = std::move(*(iter - 1));
         *iter = std::move(tmp);
      }
   }

   template<typename RandomIterator>
   void insertionSort(const RandomIterator& begin, const RandomIterator& end)
   {
      insertionSort(begin, end, std::less<decltype(*begin)>{});
   }

   // shell sort 希尔排序
   template<typename Object, class Compare=std::less<Object>>
   void shellSort(std::vector<Object>& arr, const Compare& cmp = Compare())
   {
      for(size_t gap = arr.size() / 2; gap != 0; gap /= 2)
      {
         // 间隔gap的插入排序
         for(size_t i = gap; i < arr.size(); ++i)
         {
            Object tmp = std::move(arr[i]);
            size_t j = i;
            for(; j >= gap && cmp(tmp, arr[j - gap]); j -= gap)
               arr[j] = std::move(arr[j - gap]);
            arr[j] = std::move(tmp);
         }
      }
   }

   inline size_t leftChild(size_t i)
   { return 2 * i + 1; }

   inline int leftChild(int i)
   { return 2 * i + 1; }

   template<typename Object, class Compare=std::less<Object>>
   void percDown(std::vector<Object>& arr, size_t begin, size_t end, const Compare& cmp = Compare())
   {
      size_t child = leftChild(begin);
      Object tmp;
      for(tmp = std::move(arr[begin]); child < end; )
      {
         if(child < end - 1 && cmp(arr[child], arr[child + 1]))
            ++child;
         if(cmp(tmp, arr[child]))
            arr[begin] = std::move(arr[child]);
         else
            break;
         begin = child;
         child = leftChild(begin);
      }
      arr[begin] = std::move(tmp);
   }

   // heap sort
   template<typename Object, class Compare=std::less<Object>>
   void heapSort(std::vector<Object>& arr, const Compare& cmp = Compare())
   {
      // build heap 下滤过程，建立与排序顺序相反的堆
      for (size_t i = arr.size() / 2 - 1; i > 0; --i)
         percDown(arr, i, arr.size(), cmp);
      percDown(arr, 0, arr.size(), cmp);
      // deleteTop
      for (size_t j = arr.size() - 1; j > 0; --j)
      {
         std::swap(arr[0], arr[j]); // 去top
         percDown(arr, 0, j, cmp); // 下滤
      }
   }

   template<typename Object, class Compare=std::less<Object>>
   void merge(std::vector<Object>& arr, std::vector<Object>& tmp_arr,
              size_t left, size_t right, size_t right_end, const Compare& cmp = Compare())
   {
      size_t left_end = right - 1;
      size_t tmp_pos = left;
      size_t n_elements = right_end - left + 1;

      // main loop
      // 将arr里的数字插入到tmp_arr
      while(left <= left_end && right <= right_end)
      {
         if(cmp(arr[left], arr[right]))
            tmp_arr[tmp_pos++] = std::move(arr[left++]);
         else
            tmp_arr[tmp_pos++] = std::move(arr[right++]);
      }
      while(left <= left_end)
         tmp_arr[tmp_pos++] = std::move(arr[left++]);

      while(right <= right_end)
         tmp_arr[tmp_pos++] = std::move(arr[right++]);

      // copy tmp_arr
      for(size_t i = 0; i < n_elements; ++i, --right_end)
         arr[right_end] = std::move(tmp_arr[right_end]);
   }

   // 归并排序
   template<typename Object, class Compare=std::less<Object>>
   void mergeSort(std::vector<Object>& arr, std::vector<Object>& tmp_arr,
                  size_t left, size_t right, const Compare& cmp = Compare())
   {
      if(left < right)
      {
         size_t mid = (left + right) / 2;
         mergeSort(arr, tmp_arr, left, mid, cmp);
         mergeSort(arr, tmp_arr, mid + 1, right, cmp);
         merge(arr, tmp_arr, left, mid + 1, right, cmp);
      }
   }

   template<typename Object, class Compare=std::less<Object>>
   void mergeSort(std::vector<Object>& arr, const Compare& cmp = Compare())
   {
      std::vector<Object> tmp_arr(arr.size(), Object());
      mergeSort(arr, tmp_arr, 0, arr.size() - 1, cmp);
   }

   // 采用三分中值分割法找枢轴
   template<typename Object, class Compare=std::less<Object>>
   const Object& median3(std::vector<Object>& arr, size_t left, size_t right,
                         const Compare& cmp = Compare())
   {
      size_t mid = (left + right) / 2; // 中间点
      if(cmp(arr[mid], arr[left])) // mid 与 left 逆序
         std::swap(arr[left], arr[mid]);
      if(cmp(arr[right], arr[left])) // right 与 left 逆序
         std::swap(arr[left], arr[right]);
      if(cmp(arr[right], arr[mid])) // right 与 mid逆序
         std::swap(arr[right], arr[mid]);
      // 交换之后 arr[left] arr[mid] arr[right] 有序
      std::swap(arr[mid], arr[right - 1]); // 将mid处放到right-1去
      return arr[right - 1];
   }

   template<typename Object, class Compare=std::less<Object>>
   void quickSort(std::vector<Object>& arr, size_t left, size_t right,
                  const Compare& cmp = Compare())
   {
      // 递归过程中，当子数组大小较小时，会使用插入排序直接保持有序
      if(left + 10 <= right)
      {
         // 找pivot
         const Object &pivot = median3(arr, left, right, cmp);
         // 三分中值法后 arr[left] arr[mid] arr[right] 有序
         // pivot 被放在 arr[right - 1] 处
         size_t i = left, j = right - 1;
         while (true)
         {
            while (cmp(arr[++i], pivot)); // 从头向后，直到出现逆序
            while (cmp(pivot, arr[--j])); // 从后向前，直到出现逆序
            // 处理逆序
            if (i < j)
               std::swap(arr[i], arr[j]); // 交换逆序数，arr[j] 放到pivot之前，arr[i] 放到pivot之后
            else
               break;
         }
         // 上述结束后，i,j 指向一个逆序于pivot元素
         std::swap(arr[i], arr[right - 1]); // 保存pivot
         // 枢轴不用再操作
         quickSort(arr, left, i - 1, cmp); // sort small elements
         quickSort(arr, i + 1, right, cmp); // sort large elements
      } else{ // 对于长度小于10的排序表，用直接插入排序更快
         insertionSort(arr, left, right, cmp);
      }
   }

   // 快速排序
   // 采用三分中值分割法找枢轴
   template<typename Object, class Compare=std::less<Object>>
   void quickSort(std::vector<Object>& arr, const Compare& cmp = Compare())
   {
      quickSort(arr, 0, arr.size() - 1, cmp);
   }

   // 最经典的快速排序
   template <typename Object, class Compare=std::less<Object>>
   void SORT(std::vector<Object>& arr, const Compare& cmp = Compare())
   {
      if(arr.size() < 2)
         return;
      std::vector<Object> smaller;
      std::vector<Object> same;
      std::vector<Object> larger;

      auto chosen_item = arr[arr.size() / 2]; // 取中间
      for(auto& i : arr)
      {
         if(cmp(i, chosen_item))
            smaller.push_back(std::move(i));
         else if(cmp(chosen_item, i))
            larger.push_back(std::move(i));
         else
            same.push_back(std::move(i));
      }

      SORT(smaller);
      SORT(larger);

      // 将smaller same larger数组的内容转移到arr中
      std::move(std::begin(smaller), std::end(smaller), std::begin(arr));
      std::move(std::begin(same), std::end(same), std::begin(arr) + smaller.size());
      std::move(std::begin(larger), std::end(larger), std::end(arr) - larger.size());
   }

   // 快速选择
   // Internal selection method that makes recursive calls.
   // Uses median-of-three partitioning and a cutoff of 10.
   // Places the kth smallest item in a[k-1].
   // a is an array of Comparable items.
   // left is the left-most index of the subarray.
   // right is the right-most index of the subarray.
   // k is the desired rank (1 is minimum) in the entire array.
   template<typename Object, class Compare=std::less<Object>>
   void quickSelect(std::vector<Object>& arr, size_t left, size_t right,
                    size_t k, const Compare& cmp = Compare())
   {
      if(left + 10 <= right)
      {
         const Object& pivot = median3(arr, left, right, cmp);

         size_t i = left, j = right - 1;
         while (true)
         {
            while (cmp(arr[++i], pivot));
            while (cmp(pivot, arr[--j]));
            if (i < j)
               std::swap(arr[i], arr[j]);
            else
               break;
         }

         std::swap(arr[i], arr[right - 1]);

         // 相比快速排序，快速选择只重新排其中一部分
         if(k <= i)
            quickSelect(arr, left, i - 1, k);
         else if(k > i + 1)
            quickSelect(arr, i + 1, right, k);
      } else{
         insertionSort(arr, left, right, cmp);
      }
   }

   // Quick selection algorithm.
   // Places the kth smallest item in a[k-1].
   // a is an array of Comparable items.
   // k is the desired rank (1 is minimum) in the entire array.
   template<typename Object, class Compare=std::less<Object>>
   void quickSelect(std::vector<Object>& arr, size_t k, const Compare& cmp = Compare())
   {
      quickSelect(arr, 0, arr.size() - 1, k, cmp);
   }
}

static std::vector<uint32_t> generate_input(int len) {
   static std::random_device rd;  // Will be used to obtain a seed for the random number engine
   static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
   static std::uniform_int_distribution<> dis(1, 6);

   std::vector<uint32_t> res(len);
   std::generate(res.begin(), res.end(), [&]() {
      uint32_t r = 0;
      do {
         r = static_cast<uint32_t>(dis(gen));
      } while (r <= 0);
      return r;
   });

   return res;
}

template<typename T, class Compare=std::less<T>>
static bool verify(const std::vector<T>& actual, std::vector<T> original) {
   assert(actual.size() == original.size());
   std::sort(original.begin(),original.end(), Compare{});

   for (int ii = 0; ii < actual.size(); ii++) {
      if(actual[ii] != original[ii])
         std::cerr << "the results did not match at index " << ii << std::endl;
   }

   return true;
}

#endif //TEST_CUDA_GLOBAL_POINTER_FUNCS_H
