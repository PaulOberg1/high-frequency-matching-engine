enum class Side { Buy, Sell };

enum class OrderType {
    Limit,
    Market,
    Cancel
};

enum class TimeInForce {
    GTC, //Good till cancelled
    IOC, //Immediate or cancel
    FOK  //Fill or kill
};

struct Order {
    uint64_t id;
    Side side;
    OrderType type;
    TimeInForce tif;
    uint64_t price;
    uint32_t quantity;
};