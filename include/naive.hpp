// Copyright 2026 51ddhesh
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <vector>
#include <memory>
#include <cstdint>

// Generic market processing interface
class MarketMessage {
public:
    virtual ~MarketMessage() = default;
    virtual void process() = 0;
};

// Specific implementation for adding an order
class AddOrderMessage : public MarketMessage {
    uint64_t timestamp;
    uint64_t order_id;
    uint32_t price;
    uint32_t quantity;
    char side; // Buy / Sell

public:
    AddOrderMessage(uint64_t ts, uint64_t id, uint32_t p, uint32_t q, char s) :
        timestamp(ts), order_id(id), price(p), quantity(q), side(s) {}


    void process() override {
        // Dummy state machine update logic
        asm volatile("" : : "g"(price), "g"(quantity) : "memory");
    }
};

void process_stream(const std::vector<std::shared_ptr<MarketMessage>>& stream) {
    for (const auto& msg : stream) {
        msg -> process(); // Latency trap
    }
}