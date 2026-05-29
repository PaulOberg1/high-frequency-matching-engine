#pragma once

#include <cstdint>

enum class Side
{
    Buy,
    Sell
};

enum class OrderType
{
    Limit,
    Market
};

struct Order
{
    uint64_t id{};
    Side side{};
    OrderType type{};

    double price{};
    uint32_t quantity{};

    uint64_t timestamp{};
};