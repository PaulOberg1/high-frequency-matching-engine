#pragma once

#include "Order.hpp"
#include <map>
#include <deque>

class OrderBook
{
public:
    OrderBook() = default;

    void processOrder(Order order);

private:
    std::map<double, std::deque<Order>, std::greater<double>> m_buyOrders;
    std::map<double, std::deque<Order>, std::less<double>> m_sellOrders;
};