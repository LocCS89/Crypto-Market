#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "useraccount.hpp"
#include "utility.hpp"

// Exchange class declaration
class Exchange {
private:
    // Users' accounts: username -> UserAccount
    std::map<std::string, UserAccount> users;

    // Order books: asset -> buy and sell orders
    std::map<std::string, std::vector<Order>> orderBookBuy;  // Buy orders
    std::map<std::string, std::vector<Order>> orderBookSell; // Sell orders

    // Completed trades
    std::vector<Trade> tradeHistory;

public:
    Exchange() = default;
    
    void MakeDeposit(const std::string &username, const std::string &asset, int amount);
    void PrintUserPortfolios(std::ostream &os) const;
    bool MakeWithdrawal(const std::string &username, const std::string &asset, int amount);
    bool AddOrder(Order order);
    void PrintUsersOrders(std::ostream &out) const;
    void PrintTradeHistory(std::ostream &os) const;
    void PrintBidAskSpread(std::ostream &os) const;

private:
    void MatchOrders(Order order);
};
