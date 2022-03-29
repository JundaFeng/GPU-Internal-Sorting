cmake_minimum_required(VERSION 3.20)

project(gpuSorting LANGUAGES CXX CUDA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(VERBOSE_BUILD ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CUDA_STANDARD 14)
set(DEFAULT_BUILD_TYPE "Release")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(gpu_internal_sorting
        main.cu
        funcs.h
        kernels.cuh
        common/fmt.hpp
        common/catch.hpp
        common/clara.hpp
        common/utils.hpp
        common/uniform_random.h
        )

target_link_libraries(gpu_internal_sorting ${CUDA_LIBRARIES})
include_directories(gpu_internal_sorting ${CUDA_INCLUDE_DIRS})
set_target_properties(gpu_internal_sorting PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
        CUDA_RESOLVE_DEVICE_SYMBOLS ON)
