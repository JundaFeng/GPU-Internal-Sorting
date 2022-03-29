#include "funcs.h"
#include "kernels.cuh"
#include "common/uniform_random.h"

#define PRINT(...) LOG(info, std::string(fmt::format(__VA_ARGS__)))

using namespace std;
using namespace DS;


template<typename T>
void permute(vector<T> &a)
{
   static UniformRandom r;

   for (std::size_t j = 1; j < a.size(); ++j)
      swap(a[j], a[r.nextInt(0, j)]);
}

int main() {
   int NUM_ITEMS = 10000;
   auto hostInput = generate_input(NUM_ITEMS);

   timer_start("Insertion Sort");

   // permute(hostInput);
   auto hostOutput = insertionSort(hostInput);
   verify(hostOutput, hostInput);

   timer_stop();
   return 0;
}

