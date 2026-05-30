#include "OrderBook.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

uint64_t percentile(std::vector<uint64_t>& values, double p)
{
    std::sort(values.begin(), values.end());

    size_t index = static_cast<size_t>((p / 100.0) * values.size());

    if (index >= values.size())
        index = values.size() - 1;

    return values[index];
}

int main()
{
    OrderBook book;

    constexpr size_t orderCount = 1'000'000;

    std::vector<uint64_t> latenciesNs;
    latenciesNs.reserve(orderCount);

    std::mt19937_64 rng(12345);

    std::uniform_int_distribution<uint64_t> priceDist(95, 105);
    std::uniform_int_distribution<uint32_t> qtyDist(1, 100);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> typeDist(0, 99);

    uint64_t nextOrderId = 1;

    auto benchmarkStart = Clock::now();

    for (size_t i = 0; i < orderCount; ++i)
    {
        int typeRoll = typeDist(rng);

        Order order;
        order.id = nextOrderId++;
        order.side = sideDist(rng) == 0 ? Side::Buy : Side::Sell;
        order.price = priceDist(rng);
        order.quantity = qtyDist(rng);

        if (typeRoll < 80) {
            order.type = OrderType::Limit;
            order.tif = TimeInForce::GTC;
        }
        else if (typeRoll < 90) {
            order.type = OrderType::Limit;
            order.tif = TimeInForce::IOC;
        }
        else if (typeRoll < 95) {
            order.type = OrderType::Limit;
            order.tif = TimeInForce::FOK;
        }
        else {
            order.type = OrderType::Market;
            order.tif = TimeInForce::IOC;
        }

        auto start = Clock::now();

        auto trades = book.processOrder(order);

        auto end = Clock::now();

        auto latency =
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        latenciesNs.push_back(static_cast<uint64_t>(latency));
    }

    auto benchmarkEnd = Clock::now();

    double seconds =
        std::chrono::duration<double>(benchmarkEnd - benchmarkStart).count();

    double throughput = static_cast<double>(orderCount) / seconds;

    std::cout << "Orders: " << orderCount << '\n';
    std::cout << "Throughput: " << throughput << " orders/sec\n";
    std::cout << "p50:  " << percentile(latenciesNs, 50.0) << " ns\n";
    std::cout << "p99:  " << percentile(latenciesNs, 99.0) << " ns\n";
    std::cout << "p999: " << percentile(latenciesNs, 99.9) << " ns\n";
}