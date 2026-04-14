#pragma once
#include <vector>
#include <cstdint>
#include <immintrin.h> // Intel AVX/SSE Intrinsics

// 1. Struct of Arrays (SoA) layout
struct MarketDataSoA {
    // alignas(64) ensures vectors align perfectly with L1 cache line boundaries
    alignas(64) std::vector<uint64_t> timestamps;
    alignas(64) std::vector<uint64_t> order_ids;
    alignas(64) std::vector<uint32_t> prices;
    alignas(64) std::vector<uint32_t> quantities;
    alignas(64) std::vector<uint8_t> types; 

    void reserve(size_t size) {
        timestamps.reserve(size); order_ids.reserve(size);
        prices.reserve(size); quantities.reserve(size); types.reserve(size);
    }
};

// 2. The SIMD Processing Loop
inline void process_stream_simd(const MarketDataSoA& book, size_t size) {
    const uint32_t* __restrict__ qty_ptr = book.quantities.data();
    
    // AVX2 uses 256-bit registers. 
    // 256 bits / 32 bits (uint32_t) = 8 items processed per instruction
    size_t i = 0;
    
    // A dummy accumulator register to hold our results
    __m256i vector_accumulator = _mm256_setzero_si256();

    for (; i + 8 <= size; i += 8) {
        // Load 8 quantities from memory directly into a 256-bit register simultaneously
        __m256i q_vec = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(qty_ptr + i));
        
        // Dummy SIMD operation: Add the 8 quantities to our accumulator in one clock cycle
        vector_accumulator = _mm256_add_epi32(vector_accumulator, q_vec);
    }

    // Tail loop: Handle any remaining items that don't fit perfectly into a block of 8
    for (; i < size; ++i) {
        asm volatile("" : : "g"(qty_ptr[i]) : "memory");
    }
    
    // Prevent the compiler from optimizing away our SIMD calculations
    asm volatile("" : : "x"(vector_accumulator) : "memory");
}