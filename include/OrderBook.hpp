#pragma once

#include "Order.hpp"

class OrderBook
{
public:
    OrderBook() = default;

    void processOrder(const Order& order);

private:
};