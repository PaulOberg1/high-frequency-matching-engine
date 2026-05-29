#pragma once

#include "OrderBook.hpp"

#include <string>
#include <unordered_map>

class MatchingEngine
{
public:
    MatchingEngine() = default;

    void submitOrder(
        const std::string& symbol,
        const Order& order);

private:
    std::unordered_map<std::string, OrderBook> m_orderBooks;
};