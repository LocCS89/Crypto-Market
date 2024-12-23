#include "utility.hpp"

// Equality operator (operator==)
bool Order::operator==(const Order& other) const {
    return (username == other.username) && (side == other.side) &&
           (asset == other.asset) && (amount == other.amount) && (price == other.price);
}

bool Order::operator<(const Order& other) const {
    return price < other.price; 
}

bool Order::operator>(const Order& other) const {
    return price > other.price; 
}

// Output operator (operator<<)
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.side << " " << order.amount << " " << order.asset
       << " at " << order.price << " USD by " << order.username;
    return os;
}

