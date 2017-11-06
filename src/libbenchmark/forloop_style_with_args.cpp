#include <benchmark/benchmark.h>
#include <vector>
#include <iterator>

template <typename T>
double sum(const T* _data, std::size_t _len){

    double value = 0;
    for(std::size_t i = 0;i<_len;++i)
        value += _data[i];

    return value;
}

template <typename container_type>
double sum(const container_type& _data){

    typedef typename container_type::value_type value_t;

    double value = 0;
    for(const value_t& el : _data)
        value += el;

    return value;
}

static void BM_integer_index(benchmark::State& state) {

    const std::size_t len = state.range(0);
    std::vector<float> values(len,0.f);
    double result = 0;

    for (auto _ : state){
        benchmark::DoNotOptimize(result = sum(values.data(), len));
    }
}
// Register the function as a benchmark
BENCHMARK(BM_integer_index)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(128<<10)->Arg(1<<20)->Arg(128<<20);

static void BM_range_based(benchmark::State& state) {

    const std::size_t len = state.range(0);
    std::vector<float> values(len,0.f);
    double result = 0;

    for (auto _ : state){
        benchmark::DoNotOptimize(result = sum(values));
    }

}
BENCHMARK(BM_range_based)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(128<<10)->Arg(1<<20)->Arg(128<<20);

BENCHMARK_MAIN();
