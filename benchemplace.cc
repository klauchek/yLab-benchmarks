//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Benchmark for push vs emplace
// Using emplace looks cleaner and may be faster
//-----------------------------------------------------------------------------
#include <benchmark/benchmark.h>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

constexpr int NSZ = 10000;

#ifndef REAL_NOINLINE
#define NOINLINE
#else
#define NOINLINE __attribute__((noinline))
#endif

struct Heavy {
  std::vector<int> v;
  Heavy(int n) NOINLINE : v(n) {}

  // inefective copy-ctor (with some work inside)
  Heavy(const Heavy &rhs) NOINLINE : v(rhs.v) {
    for (auto &elt : v)
      elt *= 17;
  }
  // inefective move-ctor (delegate to copy)
  Heavy(Heavy &&rhs) : Heavy(rhs) {}
  Heavy &operator=(const Heavy &rhs) = delete;
  Heavy &operator=(Heavy &&rhs) = delete;
  ~Heavy() = default;
};

static void bench_push_back(benchmark::State& state) {
  std::vector<Heavy> v_push;
  v_push.reserve(state.range(0));
  for(auto _ : state) {
    for (int i = 0; i < state.range(0); ++i)
      v_push.push_back(Heavy{NSZ});
  }
  long long aggpush = 0;
  for (auto &&p : v_push)
  aggpush += std::accumulate(p.v.begin(), p.v.end(), 0);
}
BENCHMARK(bench_push_back)->DenseRange(10000, 90000, 10000)->Unit(benchmark::kMillisecond);

static void bench_emplace(benchmark::State& state) {
  std::vector<Heavy> v_emplace;
  v_emplace.reserve(state.range(0));
  for(auto _ : state) {
    for (int i = 0; i < state.range(0); ++i)
      v_emplace.emplace_back(NSZ);
  }
  long long aggemplace = 0;
  for (auto &&p : v_emplace)
  aggemplace += std::accumulate(p.v.begin(), p.v.end(), 0);
}
BENCHMARK(bench_emplace)->DenseRange(10000, 90000, 10000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();