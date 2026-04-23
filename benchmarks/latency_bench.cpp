// Copyright 2026 51ddhesh
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <benchmark/benchmark.h>
#include <vector>
#include <memory>
// #include <naive.hpp>
// #include <data_oriented.hpp>
// #include <static_resolution_branch_hints.hpp>
// #include <simd_vectorized.hpp>
#include <metal.hpp>

// class NaiveMarketDataFixture : public benchmark::Fixture {
// public:
//     std::vector<std::shared_ptr<MarketMessage>> stream;

//     void SetUp(const ::benchmark::State& state) {
//         size_t num_messages = state.range(0);
//         stream.reserve(num_messages);

//         for (size_t i = 0; i < num_messages; i++) {
//             stream.push_back(std::make_shared<AddOrderMessage>(
//                 i, i, 100, 10, 'B'
//             ));
//         }
//     }

//     void TearDown(const ::benchmark::State& state) {
//         stream.clear();
//     }
// };


// BENCHMARK_DEFINE_F(NaiveMarketDataFixture, Naive_Processing)(benchmark::State& state) {
//     for (auto _ : state) {
//         // Test the function
//         process_stream(stream);

//         // Prevent the compiler from optimizing away the loop
//         // since the dummy process() does not have any visible side effects
//         benchmark::ClobberMemory();
//     }

//     state.SetItemsProcessed(state.iterations() * state.range(0));
// }

// BENCHMARK_REGISTER_F(NaiveMarketDataFixture, Naive_Processing)
//     -> RangeMultiplier(10)
//     -> Range(10000, 1000000);


// class DataOrientedMarketDataFixture : public benchmark::Fixture {
// public:
//     std::vector<MarketMessagePod> stream;

//     void SetUp(const ::benchmark::State& state) {
//         size_t num_messages = state.range(0);
        
//         // Pre-allocate contiguous memory
//         stream.reserve(num_messages);
        
//         for (size_t i = 0; i < num_messages; ++i) {
//             stream.push_back({
//                 static_cast<uint64_t>(i), 
//                 static_cast<uint64_t>(i), 
//                 100, 
//                 10, 
//                 MsgType::ADD, 
//                 'B'
//             });
//         }
//     }

//     void TearDown(const ::benchmark::State& state) {
//         stream.clear();
//         stream.shrink_to_fit();
//     }
// };

// BENCHMARK_DEFINE_F(DataOrientedMarketDataFixture, DoD_Processing)(benchmark::State& state) {
//     for (auto _ : state) {
//         process_stream(stream);
//         benchmark::ClobberMemory();
//     }
//     state.SetItemsProcessed(state.iterations() * state.range(0));
// }

// BENCHMARK_REGISTER_F(DataOrientedMarketDataFixture, DoD_Processing)
//     -> RangeMultiplier(10)
//     -> Range(10000, 1000000);

// class SRBH_MarketDataFixture : public benchmark::Fixture {
// public:
//     std::vector<MarketMessagePod> stream;
//     FastOrderBook book;

//     void SetUp(const ::benchmark::State& state) {
//         size_t num_messages = state.range(0);
//         stream.reserve(num_messages);
        
//         for (size_t i = 0; i < num_messages; ++i) {
//             // Simulating a realistic ratio: 90% Adds, 10% Cancels
//             MsgType type = (i % 10 == 0) ? MsgType::CANCEL : MsgType::ADD;
            
//             stream.push_back({
//                 static_cast<uint64_t>(i), 
//                 static_cast<uint64_t>(i), 
//                 100, 
//                 10, 
//                 type, 
//                 'B'
//             });
//         }
//     }

//     void TearDown(const ::benchmark::State& state) {
//         stream.clear();
//         stream.shrink_to_fit();
//     }
// };

// BENCHMARK_DEFINE_F(SRBH_MarketDataFixture, Static_Processing)(benchmark::State& state) {
//     for (auto _ : state) {
//         process_stream(stream, book);
//         benchmark::ClobberMemory();
//     }
//     state.SetItemsProcessed(state.iterations() * state.range(0));
// }

// BENCHMARK_REGISTER_F(SRBH_MarketDataFixture, Static_Processing)
//     -> RangeMultiplier(10)
//     -> Range(10000, 1000000);

// class SIMD_MarketDataFixture : public benchmark::Fixture {
// public:
//     MarketDataSoA stream;

//     void SetUp(const ::benchmark::State& state) {
//         size_t num_messages = state.range(0);
//         stream.reserve(num_messages);
        
//         for (size_t i = 0; i < num_messages; ++i) {
//             stream.timestamps.push_back(i);
//             stream.order_ids.push_back(i);
//             stream.prices.push_back(100);
//             stream.quantities.push_back(10);
//             stream.types.push_back(0); // 0 = ADD
//         }
//     }

//     void TearDown(const ::benchmark::State& state) {
//         stream.timestamps.clear(); stream.timestamps.shrink_to_fit();
//         stream.order_ids.clear(); stream.order_ids.shrink_to_fit();
//         stream.prices.clear(); stream.prices.shrink_to_fit();
//         stream.quantities.clear(); stream.quantities.shrink_to_fit();
//         stream.types.clear(); stream.types.shrink_to_fit();
//     }
// };

// BENCHMARK_DEFINE_F(SIMD_MarketDataFixture, SIMD_Processing)(benchmark::State& state) {
//     for (auto _ : state) {
//         process_stream_simd(stream, state.range(0));
//         benchmark::ClobberMemory();
//     }
//     state.SetItemsProcessed(state.iterations() * state.range(0));
// }

// BENCHMARK_REGISTER_F(SIMD_MarketDataFixture, SIMD_Processing)
//     -> RangeMultiplier(10)
//     -> Range(10000, 1000000);

class Metal_MarketDataFixture : public benchmark::Fixture {
    public:
    MarketDataSoA stream;
    
    void SetUp(const ::benchmark::State& state) {
        size_t num_messages = state.range(0);
        stream.reserve(num_messages);
        
        for (size_t i = 0; i < num_messages; ++i) {
            stream.timestamps.push_back(i);
            stream.order_ids.push_back(i);
            stream.prices.push_back(100);
            stream.quantities.push_back(10);
            stream.types.push_back(0); // 0 = ADD
        }
    }
    
    void TearDown(const ::benchmark::State& state) {
        stream.timestamps.clear(); stream.timestamps.shrink_to_fit();
        stream.order_ids.clear(); stream.order_ids.shrink_to_fit();
        stream.prices.clear(); stream.prices.shrink_to_fit();
        stream.quantities.clear(); stream.quantities.shrink_to_fit();
        stream.types.clear(); stream.types.shrink_to_fit();
    }
};

BENCHMARK_DEFINE_F(Metal_MarketDataFixture, Metal_SIMD_Processing)(benchmark::State& state) {
    // Pin to CPU Core 2 (Adjust this if Core 2 is your OS critical core)
    constexpr int TARGET_CORE = 2; 

    for (auto _ : state) {
        process_stream(stream, state.range(0), TARGET_CORE);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}
        
BENCHMARK_REGISTER_F(Metal_MarketDataFixture, Metal_SIMD_Processing)
    -> RangeMultiplier(10)
    -> Range(10000, 1000000);
        
BENCHMARK_MAIN();