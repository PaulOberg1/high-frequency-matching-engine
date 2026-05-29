#include "MatchingEngine.hpp"

#include <iostream>

int main()
{
    MatchingEngine engine;

    Order order{
        .id = 1,
        .side = Side::Buy,
        .type = OrderType::Limit,
        .price = 100.0,
        .quantity = 50,
        .timestamp = 1
    };

    engine.submitOrder("AAPL", order);

    std::cout << "Matching Engine Started\n";

    return 0;
}