#include "useraccount.hpp"

UserAccount::UserAccount(const std::string &username): username(username) {};

bool UserAccount::deposit(const std::string &asset, int amount){
    portfolio[asset] += amount;
    return true;
}

bool UserAccount::withdraw(const std::string &asset, int amount){
    if(portfolio[asset] >= amount){
        portfolio[asset] -= amount;
        return true;
    }
    return false;
}

bool UserAccount::hasSufficientBalance(const std::string &asset, int amount) const {
    auto it = portfolio.find(asset);
    return it != portfolio.end() && it->second >= amount;
}