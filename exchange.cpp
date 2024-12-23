#include "exchange.hpp"
#include <sstream>
#include <set>
#include <cassert> 
// MakeDeposit
void Exchange::MakeDeposit(const std::string &username, const std::string &asset, int amount){
    if(users.find(username) == users.end()){
        users[username] = UserAccount(username);
    }
    users[username].deposit(asset, amount);
}

// PrintUserPortfolios
void Exchange::PrintUserPortfolios(std::ostream &os) const {
    os << "User Portfolios (in alphabetical order):\n";
    for (const auto& pair : users) {
        const UserAccount& user = pair.second;
        os << pair.first << "'s Portfolio: ";
        for (const auto& item : user.portfolio) {
            if (item.second > 0) {  
                os << item.second << " " << item.first;
                os << ", ";
            }
        }
        os << "\n";  
    }
}

// MakeWithdrawal
bool Exchange::MakeWithdrawal(const std::string &username, const std::string &asset, int amount){
    auto it = users.find(username);
    if(it != users.end()){
        return (it->second).withdraw(asset, amount);
    }
    return false;
}

bool Exchange::AddOrder(Order order) {
    UserAccount& user = users[order.username];

    if (order.side == "Buy") {
        int totalUSD = order.price * order.amount;

        if (!user.hasSufficientBalance("USD", totalUSD)) {
            return false;  // Not enough USD
        }
        user.portfolio["USD"] -= totalUSD;

        // will add to open orders after matching
        MatchOrders(order);
    }

    else if (order.side == "Sell") {
        if (!user.hasSufficientBalance(order.asset, order.amount)) {
            return false;  // Not enough assets
        }
        user.portfolio[order.asset] -= order.amount;

        // will add to open orders after matching
        MatchOrders(order);
    }

    return true;  
}

void Exchange::ProcessOrders(std::string side, Order taker, std::deque<Order>& takerBook, 
                            std::deque<Order>& makerBook) {
    UserAccount& takerUser = users[taker.username];

    while (makerBook.size()) {
        if (taker.amount == 0) break;

        Order maker = makerBook.front();

        if (side == "Sell" && taker.price > maker.price) break;
        if (side == "Buy" && taker.price < maker.price) break;

        int amount = std::min(taker.amount, maker.amount);
        int totalPrice = amount * taker.price;
        UserAccount& makerUser = users[maker.username];

        std::string buyer, seller;

        if (side == "Sell") {
            takerUser.portfolio["USD"] += totalPrice;
            makerUser.portfolio[taker.asset] += amount;
            buyer = maker.username;
            seller = taker.username;
        }
        else if (side == "Buy") {
            takerUser.portfolio[taker.asset] += amount;
            makerUser.portfolio["USD"] += totalPrice;
            buyer = taker.username;
            seller = maker.username;
        }

        // update trade history
        Trade trade = {buyer, seller, taker.asset, amount, taker.price};
        tradeHistory.push_back(trade);

        makerUser.filledOrders.push_back(Order(maker.username, maker.side, taker.asset, amount, taker.price));
        makerBook.pop_front(); // delete (partially) completed maker order from queue

        // check if taker order is filled 
        if (taker.amount == amount) {
            takerUser.filledOrders.push_back(taker);
            taker.amount -= amount;
        }

        // if taker.amount still > 0, continue till !makerBook.size()
        if (taker.amount > amount) {
            // add partially fulfilled orders to filled orders
            Order partiallyFulfilledOrder = taker;
            partiallyFulfilledOrder.amount = amount;

            takerUser.filledOrders.push_back(partiallyFulfilledOrder);
            taker.amount -= amount;
        }
        
        // if maker order is completed, delete from open orders
        if (maker.amount == amount)
            makerUser.openOrders.erase(std::find(makerUser.openOrders.begin(), makerUser.openOrders.end(), maker));

        // if maker.amount > amount: change amount of maker in-place
        if (maker.amount > amount) {
            auto pos = std::find(makerUser.openOrders.begin(), makerUser.openOrders.end(), maker) - makerUser.openOrders.begin();
            makerUser.openOrders[pos].amount -= amount;

            // add partial order to book 
            makerBook.push_back(makerUser.openOrders[pos]);
            break; // since maker.amount > amount, taker order is completely filled 
        }
    } 

    // taker has now become a maker order
    // can safely add to book
    if (taker.amount > 0) {
        takerUser.openOrders.push_back(taker);
        takerBook.push_back(taker);
    }
}

void Exchange::MatchOrders(Order taker) {
    if (taker.side == "Buy") {
        auto& takerBook = makerBookBuy[taker.asset];
        auto& makerBook = makerBookSell[taker.asset];
        Exchange::ProcessOrders("Buy", taker, takerBook, makerBook);
    } 
    else if (taker.side == "Sell") {
        auto& takerBook = makerBookSell[taker.asset];
        auto& makerBook = makerBookBuy[taker.asset];
        Exchange::ProcessOrders("Sell", taker, takerBook, makerBook);
    }

    // sort the order book 
    std::sort(makerBookBuy[taker.asset].begin(), makerBookBuy[taker.asset].end(), std::greater<Order>());
    std::sort(makerBookSell[taker.asset].begin(), makerBookSell[taker.asset].end());
}

void Exchange::PrintUsersOrders(std::ostream& out) const {
    out << "Users Orders (in alphabetical order):\n";
    for (const auto& userPair : users) {
        const UserAccount& user = userPair.second;
        out << user.username << "'s Open Orders (in chronological order):\n";
        for (const auto& order : user.openOrders) {
            out << order.side << " " << order.amount << " " << order.asset << " at " << order.price << " USD by " << order.username << std::endl;
        }
        out << user.username << "'s Filled Orders (in chronological order):\n";
        for (const auto& order : user.filledOrders) {
            out << order.side << " " << order.amount << " " << order.asset << " at " << order.price << " USD by " << order.username << std::endl;
        }
    }
}


// PrintTradeHistory
void Exchange::PrintTradeHistory(std::ostream &os) const {
    os << "Trade History (in chronological order):\n";
    for(const auto &trade : tradeHistory){
        os << trade.buyer_username << " Bought " << trade.amount << " of " << trade.asset 
           << " From " << trade.seller_username << " for " << trade.price << " USD\n";
    }
}


void Exchange::PrintBidAskSpread(std::ostream &os) const {
    os << "Asset Bid Ask Spread (in alphabetical order):\n";

    // Iterate through all assets
    std::vector<std::string> assets;
    for (const auto& pair : makerBookBuy) {
        assets.push_back(pair.first);
    }
    for (const auto& pair : makerBookSell) {
        assets.push_back(pair.first);
    }

    // Remove duplicates
    std::sort(assets.begin(), assets.end());
    assets.erase(std::unique(assets.begin(), assets.end()), assets.end());

    for (const std::string& asset : assets) {
        std::string highestBuy = "NA";
        auto buyIt = makerBookBuy.find(asset);  
        if (buyIt != makerBookBuy.end() && !buyIt->second.empty()) {
            highestBuy = std::to_string(buyIt->second.front().price);
        }

        std::string lowestSell = "NA";
        auto sellIt = makerBookSell.find(asset); 
        if (sellIt != makerBookSell.end() && !sellIt->second.empty()) {
            lowestSell = std::to_string(sellIt->second.front().price);
        }

        os << asset << ": Highest Open Buy = " << highestBuy << " USD and Lowest Open Sell = " << lowestSell << " USD\n";
    }
}


