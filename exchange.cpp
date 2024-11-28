#include "exchange.hpp"
#include <sstream>
#include <set>
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
    auto& user = users[order.username];

    // Handle buy orders
    if (order.side == "Buy") {
        int totalUSD = order.price * order.amount;

        // Check if user has enough USD to place the buy order
        if (!user.hasSufficientBalance("USD", totalUSD)) {
            return false;  // Not enough USD
        }

        // Immediately reduce USD from the user's portfolio for the committed amount
        user.portfolio["USD"] -= totalUSD;

        // Add the buy order to the user's open orders
        user.openOrders.push_back(order);

        // Add the buy order to the buy order book
        orderBookBuy[order.asset].push_back(order);
        std::sort(orderBookBuy[order.asset].begin(), orderBookBuy[order.asset].end(), [](const Order& a, const Order& b) {
            return a.price > b.price;
        });

        // Try to match with available sell orders
        MatchOrders(order);
    }
    // Handle sell orders
    else if (order.side == "Sell") {
        if (!user.hasSufficientBalance(order.asset, order.amount)) {
            return false;  // Not enough assets
        }

        // Immediately reduce the asset from the user's portfolio for the committed amount
        user.portfolio[order.asset] -= order.amount;

        // Add the sell order to the user's open orders
        user.openOrders.push_back(order);

        // Add the sell order to the sell order book
        orderBookSell[order.asset].push_back(order);
        std::sort(orderBookSell[order.asset].begin(), orderBookSell[order.asset].end(), [](const Order& a, const Order& b) {
            return a.price < b.price;
        });

        // Try to match with available buy orders
        MatchOrders(order);
    }

    return true;  // Order was successfully added
}

void Exchange::MatchOrders(Order order) {
    if (order.side == "Buy") {
        auto& sellOrders = orderBookSell[order.asset];
        for (auto& sellOrder : sellOrders) {
            if (order.price >= sellOrder.price && order.amount > 0) {
                // Match the orders
                int matchedAmount = std::min(order.amount, sellOrder.amount);
                int totalPrice = matchedAmount * order.price;

                // Update user portfolios
                auto& buyUser = users[order.username];
                auto& sellUser = users[sellOrder.username];

                sellUser.portfolio["USD"] += totalPrice;
                buyUser.portfolio[order.asset] += matchedAmount;

                // Reduce the order amounts temporarily for matching
                int remainingBuyAmount = order.amount - matchedAmount;
                int remainingSellAmount = sellOrder.amount - matchedAmount;

                // If the sell order is filled, move it to filled orders
                if (remainingSellAmount == 0) {
                    sellUser.filledOrders.push_back(Order(sellOrder.username, sellOrder.side, 
                        sellOrder.asset, matchedAmount, order.price));
                    // Remove from sell order book and user's open orders
                    sellOrders.erase(std::find(sellOrders.begin(), sellOrders.end(), sellOrder));
                    sellUser.openOrders.erase(std::find(sellUser.openOrders.begin(), sellUser.openOrders.end(), sellOrder));
                }

                // If the buy order is filled, move it to filled orders
                if (remainingBuyAmount == 0) {
                    buyUser.filledOrders.push_back(order);
                    // Remove from buy order book and user's open orders
                    orderBookBuy[order.asset].erase(std::find(orderBookBuy[order.asset].begin(), orderBookBuy[order.asset].end(), order));
                    buyUser.openOrders.erase(std::find(buyUser.openOrders.begin(), buyUser.openOrders.end(), order));
                    break;  // No need to match further if the buy order is fully filled
                }
                int boughtAmount = order.amount - remainingBuyAmount;
                // If the buy order is partially filled, update the amount left
                order.amount = remainingBuyAmount;

                // If the sell order is partially filled, update the amount left
                sellOrder.amount = remainingSellAmount;

                // If the buy order was partially filled, push a new smaller buy order with the remaining amount
                if (remainingBuyAmount > 0) {
                    // Remove the old buy order from the open orders and order book
                    orderBookBuy[order.asset].erase(std::find(orderBookBuy[order.asset].begin(), orderBookBuy[order.asset].end(), order));
                    buyUser.openOrders.erase(std::find(buyUser.openOrders.begin(), buyUser.openOrders.end(), order));
                    // Create a new buy order with the remaining amount (2 BTC)
                    Order remainingBuyOrder = order;
                    remainingBuyOrder.amount = remainingBuyAmount;
                    buyUser.openOrders.push_back(remainingBuyOrder);
                    orderBookBuy[order.asset].push_back(remainingBuyOrder);
                    
                    Order boughtOrder = order;
                    boughtOrder.amount = boughtAmount;
                    buyUser.filledOrders.push_back(boughtOrder);

                    // Sort buy orders again after pushing the new one
                    std::sort(orderBookBuy[order.asset].begin(), orderBookBuy[order.asset].end(), [](const Order& a, const Order& b) {
                        return a.price > b.price;
                    });
                }
            }
        }
    } else if (order.side == "Sell") {
        auto& buyOrders = orderBookBuy[order.asset];
        for (auto& buyOrder : buyOrders) {
            if (order.price <= buyOrder.price && order.amount > 0) {
                // Match the orders
                int matchedAmount = std::min(order.amount, buyOrder.amount);
                int totalPrice = matchedAmount * order.price;

                // Update user portfolios
                auto& buyUser = users[buyOrder.username];
                auto& sellUser = users[order.username];

                sellUser.portfolio["USD"] += totalPrice;

                buyUser.portfolio[order.asset] += matchedAmount;

                // Reduce the order amounts temporarily for matching
                int remainingSellAmount = order.amount - matchedAmount;
                int remainingBuyAmount = buyOrder.amount - matchedAmount;

                // If the buy order is filled, move it to filled orders
                if (remainingBuyAmount == 0) {
                    buyUser.filledOrders.push_back(Order(buyOrder.username, buyOrder.side, 
                        buyOrder.asset, matchedAmount, order.price));
                    // Remove from buy order book and user's open orders
                    buyOrders.erase(std::find(buyOrders.begin(), buyOrders.end(), buyOrder));
                    buyUser.openOrders.erase(std::find(buyUser.openOrders.begin(), buyUser.openOrders.end(), buyOrder));
                }

                // If the sell order is filled, move it to filled orders
                if (remainingSellAmount == 0) {
                    sellUser.filledOrders.push_back(order);
                    // Remove from sell order book and user's open orders
                    orderBookSell[order.asset].erase(std::find(orderBookSell[order.asset].begin(), orderBookSell[order.asset].end(), order));
                    sellUser.openOrders.erase(std::find(sellUser.openOrders.begin(), sellUser.openOrders.end(), order));
                    break;  // No need to match further if the sell order is fully filled
                }
                int soldAmount = order.amount - remainingSellAmount;
                // If the sell order is partially filled, update the amount left
                order.amount = remainingSellAmount;

                // If the buy order is partially filled, update the amount left
                buyOrder.amount = remainingBuyAmount;

                // If the sell order was partially filled, push a new smaller sell order with the remaining amount
                if (remainingSellAmount > 0) {
                    // Remove the old sell order from the open orders and order book
                    orderBookSell[order.asset].erase(std::find(orderBookSell[order.asset].begin(), orderBookSell[order.asset].end(), order));
                    sellUser.openOrders.erase(std::find(sellUser.openOrders.begin(), sellUser.openOrders.end(), order));

                    Order remainingSellOrder = order;
                    remainingSellOrder.amount = remainingSellAmount;
                    sellUser.openOrders.push_back(remainingSellOrder);
                    orderBookSell[order.asset].push_back(remainingSellOrder);
                    Order soldOrder = order;
                    soldOrder.amount = soldAmount;
                    sellUser.filledOrders.push_back(soldOrder);
                    // Sort sell orders again after pushing the new one
                    std::sort(orderBookSell[order.asset].begin(), orderBookSell[order.asset].end(), [](const Order& a, const Order& b) {
                        return a.price < b.price;
                    });
                }
            }
        }
    }
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
        os << trade.seller_username << " Sold " << trade.amount << " of " << trade.asset 
           << " to " << trade.buyer_username << " for " << trade.price << " USD\n";
    }
}

// PrintBidAskSpread
void Exchange::PrintBidAskSpread(std::ostream &os) const {
    os << "Asset Bid Ask Spread (in alphabetical order):\n";

    // Iterate through all assets
    std::set<std::string> assets;
    for (const auto& buyOrders : orderBookBuy) {
        assets.insert(buyOrders.first);
    }
    for (const auto& sellOrders : orderBookSell) {
        assets.insert(sellOrders.first);
    }

    for (const auto& asset : assets) {
        // Get the highest bid (Buy order) for this asset
        std::string highestBuy = "NA";
        auto buyIt = orderBookBuy.find(asset);  // Find the asset in the buy order book
        if (buyIt != orderBookBuy.end() && !buyIt->second.empty()) {
            highestBuy = std::to_string(buyIt->second.front().price);
        }

        // Get the lowest ask (Sell order) for this asset
        std::string lowestSell = "NA";
        auto sellIt = orderBookSell.find(asset);  // Find the asset in the sell order book
        if (sellIt != orderBookSell.end() && !sellIt->second.empty()) {
            lowestSell = std::to_string(sellIt->second.front().price);
        }

        os << asset << ": Highest Open Buy = " << highestBuy << " USD and Lowest Open Sell = " << lowestSell << " USD\n";
    }
}