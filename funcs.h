//
// Created by jundafeng on 3/7/22.
//
#ifndef TEST_CUDA_GLOBAL_POINTER_FUNCS_H
#define TEST_CUDA_GLOBAL_POINTER_FUNCS_H
#include <cuda.h>
#include <algorithm>
#include <random>

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


static bool verify(const std::vector<uint32_t>& expected, const std::vector<uint32_t>& actual) {
   // INFO("Verifying the output");
   SECTION("the expected and actual sizes must match") {
      REQUIRE(expected.size() == actual.size());
   }

   SECTION("the results must match") {
      for (int ii = 0; ii < expected.size(); ii++) {
         INFO("the results did not match at index " << ii);
         REQUIRE(expected[ii] == actual[ii]);
      }
   }
   return true;
}

#endif //TEST_CUDA_GLOBAL_POINTER_FUNCS_H
