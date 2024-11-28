#include "../exchange.hpp"
#include "../useraccount.hpp"
#include "../utility.hpp"

#include <iostream>
#include <sstream>
#include <cassert>

int main() {
    Exchange e;
  std::ostringstream oss;
  e.MakeDeposit("Nahum", "BTC", 1000);
  e.MakeDeposit("Dolson", "USD", 555555);
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 5, 100}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 555555 USD, \nNahum's Portfolio: 995 BTC, \n");
  e.PrintUsersOrders(std::cout);
  oss.str("");
  e.PrintUsersOrders(oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nDolson's Filled Orders (in chronological order):\nNahum's Open Orders (in chronological order):\nSell 5 BTC at 100 USD by Nahum\nNahum's Filled Orders (in chronological order):\n");
  std::cout << std::endl << std::endl;
  assert(e.AddOrder({"Dolson", "Buy", "BTC", 70, 150}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 5 BTC, 545055 USD, \nNahum's Portfolio: 995 BTC, 750 USD, \n");
  e.PrintUsersOrders(std::cout);
  oss.str("");
  e.PrintUsersOrders(oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nBuy 65 BTC at 150 USD by Dolson\nDolson's Filled Orders (in chronological order):\nBuy 5 BTC at 150 USD by Dolson\nNahum's Open Orders (in chronological order):\nNahum's Filled Orders (in chronological order):\nSell 5 BTC at 150 USD by Nahum\n");
  std::cout << std::endl << std::endl;
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 30, 125}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 35 BTC, 545055 USD, \nNahum's Portfolio: 965 BTC, 4500 USD, \n");
  e.PrintUsersOrders(std::cout);
  oss.str("");
  e.PrintUsersOrders(oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nBuy 35 BTC at 150 USD by Dolson\nDolson's Filled Orders (in chronological order):\nBuy 5 BTC at 150 USD by Dolson\nBuy 30 BTC at 125 USD by Dolson\nNahum's Open Orders (in chronological order):\nNahum's Filled Orders (in chronological order):\nSell 5 BTC at 150 USD by Nahum\nSell 30 BTC at 125 USD by Nahum\n");
  std::cout << std::endl << std::endl;
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 40, 120}));
  e.PrintTradeHistory(std::cout);
  oss.str("");
  e.PrintTradeHistory(oss);
  assert(oss.str() == "Trade History (in chronological order):\nDolson Bought 5 of BTC From Nahum for 150 USD\nDolson Bought 30 of BTC From Nahum for 125 USD\nDolson Bought 35 of BTC From Nahum for 120 USD\n");
}