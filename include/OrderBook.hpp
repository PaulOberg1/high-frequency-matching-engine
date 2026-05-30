#pragma once

#include "Order.hpp"
#include <map>
#include <deque>
#include <list>
#include <unordered_map>

class OrderBook
{
public:
    OrderBook() = default;

    void processOrder(Order order);

    void match(Order& order);

    template <typename Book>
    void matchAtPriceLevel(Order& incoming,
        typename Book::iterator levelIt,
        Book& book);

    void addToBook(Order order);
    
    void cancelOrder(uint64_t orderId);

    bool canFullyFill(const Order& order) const;

private:
    using Price = uint64_t;
    using OrderList = std::list<Order>;

    std::map<Price, OrderList, std::greater<>> m_buyOrders;
    std::map<Price, OrderList, std::less<>>    m_sellOrders;

    struct OrderLocation {
        Side side;
        Price price;
        OrderList::iterator it;
    };

    std::unordered_map<uint64_t, OrderLocation> m_orderIndex;
};