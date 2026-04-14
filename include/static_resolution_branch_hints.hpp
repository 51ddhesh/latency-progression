// Copyright 2026 51ddhesh
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once


#include <vector>
#include <cstdint>
#include <data_oriented.hpp>

template <typename Derived>
class MessageHandler {
public:
    inline void handle(const MarketMessagePod& msg) {
        static_cast<Derived*>(this) -> process_impl(msg);
    }
};

// 2. The specific implementation
class FastOrderBook : public MessageHandler<FastOrderBook> {
public:
    inline void process_impl(const MarketMessagePod& msg) {
        // 3. Branch Prediction Hints
        if (msg.type == MsgType::ADD) [[likely]] {
            asm volatile("" : : "g"(msg.price), "g"(msg.quantity) : "memory");
        } 
        else [[unlikely]] {
            asm volatile("" : : "g"(msg.order_id) : "memory");
        }
    }
};

void process_stream(const std::vector<MarketMessagePod>& stream, FastOrderBook& book) {
    const size_t size = stream.size();
    
    // Tell the compiler that the memory pointed to by 'data' doesn't overlap 
    // with anything else, unlocking aggressive vectorization/inlining
    const MarketMessagePod* __restrict__ data = stream.data();

    // Force GCC to unroll the loop by a factor of 4
    #pragma GCC unroll 4
    for (size_t i = 0; i < size; i++) {
        book.handle(data[i]);
    }
}