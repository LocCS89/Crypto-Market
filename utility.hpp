#pragma once

#include <iostream>
#include <string>

// Order structure: represents a buy or sell order
struct Order {
    std::string username;
    std::string side;  // "Buy" or "Sell"
    std::string asset; // e.g., "BTC", "ETH"
    int amount;
    int price; // Price per asset in USD

    Order() = default;

    // Constructor
    Order(const std::string& u, const std::string& s, const std::string& a, int m, int p)
        : username(u), side(s), asset(a), amount(m), price(p) {}

    // Copy constructor
    Order(const Order& other)
        : username(other.username), side(other.side), asset(other.asset), amount(other.amount), price(other.price) {}

    // Equality operator (operator==)
    bool operator==(const Order& other) const;
    bool operator<(const Order& other) const;  
    bool operator>(const Order& other) const;

    // Output operator (operator<<)
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};

// Trade structure: represents a completed trade
struct Trade {
    std::string buyer_username;  // Buyer in the trade
    std::string seller_username; // Seller in the trade
    std::string asset;           // Asset being traded
    int amount;                  // Quantity of the asset traded
    int price;                   // Price per unit in the trade
};
