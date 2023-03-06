cmake_minimum_required(VERSION 3.14)
project(yLab_benchmark)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS NO)
set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_WERROR OFF)

FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.6.1)

FetchContent_MakeAvailable(
        googlebenchmark)

add_executable(yLab_benchmark
            benchcopy-2.cc)
target_link_libraries(yLab_benchmark benchmark::benchmark)