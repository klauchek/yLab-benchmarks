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

option(TEST_EMPLACE OFF)
option(TEST_NAIVE_COPY OFF)
option(TEST_NON_NAIVE_COPY OFF)


if(TEST_EMPLACE)
    set(SRCS benchemplace.cc)
elseif(TEST_NAIVE_COPY)
    set(SRCS benchcopy.cc)
elseif(TEST_NON_NAIVE_COPY)
    set(SRCS benchcopy-2.cc)
else()
    message(FATAL_ERROR "Choose bench to run")
endif()

add_executable(yLab_benchmark ${SRCS})
target_compile_options(yLab_benchmark PRIVATE -O2)
target_link_libraries(yLab_benchmark benchmark::benchmark)