#include "OrderBook.hpp"

void OrderBook::processOrder(Order order)
{
    if (order.type == OrderType::Cancel)
    {
        cancelOrder(order.id);
        return;
    }

    if (order.tif == TimeInForce::FOK)
    {
        if (!canFullyFill(order))
            return;
    }

    match(order);

    if (order.type == OrderType::Limit &&
        order.tif == TimeInForce::GTC &&
        order.quantity > 0)
    {
        addToBook(order);
    }

    //ioc and market leftovers discarded here
}

void OrderBook::match(Order& order)
{
    if (order.side == Side::Buy)
    {
        while (order.quantity > 0 && !m_sellOrders.empty())
        {
            auto sellIt = m_sellOrders.begin();

            if (order.type == OrderType::Limit &&
                sellIt->first > order.price)
                break;

            matchAtPriceLevel(order, sellIt, m_sellOrders);
        }
    }
    else
    {
        while (order.quantity > 0 && !m_buyOrders.empty())
        {
            auto buyIt = m_buyOrders.begin();

            if (order.type == OrderType::Limit &&
                buyIt->first < order.price)
                break;

            matchAtPriceLevel(order, buyIt, m_buyOrders);
        }
    }
}

template <typename Book>
void OrderBook::matchAtPriceLevel(Order& incoming,
    typename Book::iterator levelIt,
    Book& book)
{
    auto& orders = levelIt->second;

    while (incoming.quantity > 0 && !orders.empty())
    {
        Order& resting = orders.front();

        uint32_t executed =
            std::min(incoming.quantity, resting.quantity);

        incoming.quantity -= executed;
        resting.quantity -= executed;

        //emit trade event
        //trade price=levelIt.first

        if (resting.quantity == 0)
        {
            m_orderIndex.erase(resting.id);
            orders.pop_front();
        }
    }

    if (orders.empty())
        book.erase(levelIt);
}


void OrderBook::addToBook(Order order)
{
    if (order.side == Side::Buy)
    {
        auto& level = m_buyOrders[order.price];
        level.push_back(order);

        auto it = std::prev(level.end());
        m_orderIndex[order.id] = { order.side, order.price, it };
    }
    else
    {
        auto& level = m_sellOrders[order.price];
        level.push_back(order);

        auto it = std::prev(level.end());
        m_orderIndex[order.id] = { order.side, order.price, it };
    }
}

void OrderBook::cancelOrder(uint64_t orderId)
{
    auto indexIt = m_orderIndex.find(orderId);
    if (indexIt == m_orderIndex.end())
        return;

    const OrderLocation loc = indexIt->second;

    if (loc.side == Side::Buy)
    {
        auto bookIt = m_buyOrders.find(loc.price);
        bookIt->second.erase(loc.it);

        if (bookIt->second.empty())
            m_buyOrders.erase(bookIt);
    }
    else
    {
        auto bookIt = m_sellOrders.find(loc.price);
        bookIt->second.erase(loc.it);

        if (bookIt->second.empty())
            m_sellOrders.erase(bookIt);
    }

    m_orderIndex.erase(indexIt);
}

bool OrderBook::canFullyFill(const Order& order) const
{
    uint32_t needed = order.quantity;

    if (order.side == Side::Buy)
    {
        for (const auto& [price, orders] : m_sellOrders)
        {
            if (order.type == OrderType::Limit && price > order.price)
                break;

            for (const auto& resting : orders)
            {
                needed -= std::min(needed, resting.quantity);
                if (needed == 0)
                    return true;
            }
        }
    }
    else
    {
        for (const auto& [price, orders] : m_buyOrders)
        {
            if (order.type == OrderType::Limit && price < order.price)
                break;

            for (const auto& resting : orders)
            {
                needed -= std::min(needed, resting.quantity);
                if (needed == 0)
                    return true;
            }
        }
    }

    return false;
}