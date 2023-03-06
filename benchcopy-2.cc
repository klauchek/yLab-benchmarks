//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
// Benchmark for naive copy vs memcpy
// godbolt link for simplicity: https://godbolt.org/z/GanKEGddo
//
//-----------------------------------------------------------------------------
#include <benchmark/benchmark.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstring>
#include <list>
#include <random>
#include <vector>

const int IMAX = 9;

template <typename InputIt, typename OutputIt>
OutputIt long_copy(InputIt first, InputIt last, OutputIt d_first) {
  while (first != last)
    *d_first++ = *first++;
  return d_first;
}

template <bool Type, typename In, typename Out> struct CpSel {
  static Out select(In begin, In end, Out out) {
    return long_copy(begin, end, out);
  }
};

template <typename In, typename Out> struct CpSel<true, In, Out> {
  static Out select(In begin, In end, Out out) {
    using in_type = typename std::iterator_traits<In>::value_type;
    auto sz = (end - begin) * sizeof(in_type);
    memcpy(&*out, &*begin, sz);
    return out;
  }
};

template <typename In, typename Out>
Out nonnaive_copy(In begin, In end, Out out) {
  using in_type = typename std::iterator_traits<In>::value_type;
  using out_type = typename std::iterator_traits<Out>::value_type;
  enum {
    Sel = std::is_trivially_copyable<in_type>::value &&
          std::is_trivially_copyable<out_type>::value &&
          std::is_same<in_type, out_type>::value
  };
  return CpSel<Sel, In, Out>::select(begin, end, out);
}

void fill_random(std::vector<int>& arr) {
  std::random_device rd;
  std::default_random_engine reng(rd());
  std::uniform_int_distribution<int> dist(0, IMAX);

  std::generate(arr.begin(), arr.end(), [&dist, &reng] { return dist(reng); });
}

static void bench_memcpy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  fill_random(arr);

  for(auto _ : state)
    std::memcpy(arrcopy.data(), arr.data(), state.range(0) * sizeof(int));

}
BENCHMARK(bench_memcpy)->DenseRange(100000000, 250000000, 50000000)->Unit(benchmark::kMillisecond);


static void bench_nonnaive_copy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  fill_random(arr);

  for(auto _ : state)
    nonnaive_copy(arr.begin(), arr.end(), arrcopy.begin());

}
BENCHMARK(bench_nonnaive_copy)->DenseRange(100000000, 250000000, 50000000)->Unit(benchmark::kMillisecond);


static void bench_std_copy(benchmark::State& state) {
  std::vector<int> arr(state.range(0));
  std::vector<int> arrcopy(state.range(0));

  fill_random(arr);

  for(auto _ : state)
    std::copy(arr.begin(), arr.end(), arrcopy.begin());

}
BENCHMARK(bench_std_copy)->DenseRange(100000000, 250000000, 50000000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();