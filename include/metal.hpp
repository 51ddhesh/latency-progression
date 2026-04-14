// Copyright 2026 51ddhesh
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <pthread.h>
#include <sched.h>
#include <stdexcept>
#include <iostream>
#include <simd_vectorized.hpp> // Reusing our peak SIMD logic

class HardwareIsolator {
public:
    static void pin_thread_to_core(int core_id) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        
        pthread_t current_thread = pthread_self();
        if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
            std::cerr << "WARNING: Failed to pin thread to core " << core_id << "\n";
        }
    }
};

// The V4 processing function simply wraps V3 but ensures we are pinned
inline void process_stream(const MarketDataSoA& book, size_t size, int target_core) {
    HardwareIsolator::pin_thread_to_core(target_core);
    
    // Execute the hyper-optimized SIMD loop
    process_stream_simd(book, size);
}