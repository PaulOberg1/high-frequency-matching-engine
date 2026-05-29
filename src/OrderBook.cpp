#include "OrderBook.hpp"
void OrderBook::processOrder(Order order)
{
    if (order.side == Side::Buy)
    {
        auto sellIt = m_sellOrders.begin();

        while (order.quantity > 0 &&
            sellIt != m_sellOrders.end() &&
            (order.type == OrderType::Market || sellIt->first <= order.price))
        {
            auto& orderQueue = sellIt->second;

            while (order.quantity > 0 && !orderQueue.empty())
            {
                Order& sellOrder = orderQueue.front();

                uint32_t executedQuantity =
                    std::min(order.quantity, sellOrder.quantity);

                order.quantity -= executedQuantity;
                sellOrder.quantity -= executedQuantity;

                if (sellOrder.quantity == 0)
                    orderQueue.pop_front();
            }

            if (orderQueue.empty())
                sellIt = m_sellOrders.erase(sellIt);
            else
                break;
        }

        if (order.type == OrderType::Limit && order.quantity > 0)
        {
            m_buyOrders[order.price].push_back(order);
        }
    }
}