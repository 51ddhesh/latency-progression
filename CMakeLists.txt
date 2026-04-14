cmake_minimum_required(VERSION 3.20)
project(latency_evolution CXX)

# 1. Stop Benchmark from downloading GTest (Fixes the CMake < 3.5 error)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing" FORCE)

# 2. Fetch Google Benchmark
include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.9.1 
)
FetchContent_MakeAvailable(benchmark)

# 3. Define our target
add_executable(latency_bench benchmarks/latency_bench.cpp)

# 4. Modern Target-Centric Configuration
target_compile_features(latency_bench PRIVATE cxx_std_20)
target_include_directories(latency_bench PRIVATE include)

# Apply extreme optimizations ONLY to our target
target_compile_options(latency_bench PRIVATE
    -O3 
    -march=native 
    -Wall 
    -Wextra
)

# Modern way to enable Link Time Optimization (LTO)
set_target_properties(latency_bench PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

target_link_libraries(latency_bench PRIVATE benchmark benchmark_main)