// Copyright 2026 51ddhesh
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once


#include <vector>
#include <cstdint>

enum class MsgType : uint8_t {
    ADD,
    CANCEL,
    MODIFY
};

struct MarketMessagePod {
    uint64_t timestamp;
    uint64_t order_id;
    uint32_t price;
    uint32_t quantity;
    MsgType type;
    char side;
};

void process_stream(const std::vector<MarketMessagePod>& stream) {
    for (const auto& msg : stream) {
        // Switch-case replaces virtual dispatch
        switch (msg.type) {
            case MsgType::ADD:
                // Dummy state machine update
                asm volatile("" : : "g"(msg.price), "g"(msg.quantity) : "memory");
                break;
            case MsgType::CANCEL:
            case MsgType::MODIFY:
                break;
        }
    }
}