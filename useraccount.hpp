#pragma once

#include <string>
#include <map>
#include <vector>
#include <map>
#include "utility.hpp"

class UserAccount {
public:
    std::string username;
    std::map<std::string, int> portfolio;  // Asset -> amount
    std::vector<Order> openOrders;  // Orders that are not yet filled
    std::vector<Order> filledOrders;        // User's active orders

    // Constructor
    UserAccount(const std::string &username);
    UserAccount() = default;

    // Deposit assets into the user's portfolio
    bool deposit(const std::string &asset, int amount);

    // Withdraw assets from the user's portfolio
    bool withdraw(const std::string &asset, int amount);

    // Check if the user has sufficient balance of a specific asset
    bool hasSufficientBalance(const std::string &asset, int amount) const;
};
