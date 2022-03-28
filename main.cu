#include "funcs.h"
#include "kernels.cuh"

#define PRINT(...) LOG(info, std::string(fmt::format(__VA_ARGS__)))


int eval(int inputLength = 10000) {
   timer_start("");
   auto hostInput1 = generate_input(inputLength);
   auto hostInput2 = generate_input(inputLength);
   auto gpuInput1 = hostInput1;
   auto gpuInput2 = hostInput2;

   const size_t byteCount = inputLength * sizeof(uint32_t);


   timer_stop();
   return 0;
}

TEST_CASE("Vec_Add", "[vec_add]") {
   SECTION("[inputSize:161910]") {
      eval(161910);
   }

}
