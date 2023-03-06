//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Benchmark for naive copy vs memcpy
//-----------------------------------------------------------------------------
#include <benchmark/benchmark.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <list>
#include <random>
#include <vector>

const int IMAX = 9;

template <typename InputIt, typename OutputIt>
OutputIt naive_copy(InputIt first, InputIt last, OutputIt d_first) {
  while (first != last)
    *d_first++ = *first++;
  return d_first;
}

  // // sanity: do we have mismatch (we shall not)
  // auto mism = std::mismatch(arrcopy.begin(), arrcopy.end(), arrcopy2.begin());
  // if (mism.first != arrcopy.end() || mism.second != arrcopy2.end()) {
  //   std::cout << "mismatch: " << *mism.first << " vs " << *mism.second
  //             << std::endl;
  //   std::cout << "at: " << std::distance(arrcopy.begin(), mism.first)
  //             << std::endl;
  // }



static void bench_memcpy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, IMAX);

  std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });

  for(auto _ : state)
    std::memcpy(arrcopy.data(), arr.data(), state.range(0) * sizeof(int));

}
BENCHMARK(bench_memcpy)->DenseRange(100000000, 200000000, 50000000)->Unit(benchmark::kMillisecond);


static void bench_naive_copy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, IMAX);

  std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });

  for(auto _ : state)
    naive_copy(arr.begin(), arr.end(), arrcopy.begin());

}
BENCHMARK(bench_naive_copy)->DenseRange(100000000, 200000000, 50000000)->Unit(benchmark::kMillisecond);


static void bench_std_copy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, IMAX);

  std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });

  for(auto _ : state)
    std::copy(arr.begin(), arr.end(), arrcopy.begin());

}
BENCHMARK(bench_std_copy)->DenseRange(100000000, 200000000, 50000000)->Unit(benchmark::kMillisecond);



BENCHMARK_MAIN();