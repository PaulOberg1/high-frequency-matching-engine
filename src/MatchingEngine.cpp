#include "MatchingEngine.hpp"

void MatchingEngine::submitOrder(
    const std::string& symbol,
    const Order& order)
{
    m_orderBooks[symbol].processOrder(order);
}