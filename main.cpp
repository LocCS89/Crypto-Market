#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include "exchange.hpp"
#include "useraccount.hpp"
#include "utility.hpp"

int main() {
  Exchange e;
  std::ostringstream oss;
  e.MakeDeposit("Nahum", "BTC", 10);
  e.MakeDeposit("Dolson", "USD", 5555);
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 5, 100}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 5555 USD, \nNahum's Portfolio: 5 BTC, \n");
  e.PrintUsersOrders(std::cout);
  oss.str("");
  e.PrintUsersOrders(oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nDolson's Filled Orders (in chronological order):\nNahum's Open Orders (in chronological order):\nSell 5 BTC at 100 USD by Nahum\nNahum's Filled Orders (in chronological order):\n");
  std::cout << std::endl << std::endl;
  assert(e.AddOrder({"Dolson", "Buy", "BTC", 7, 100}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 5 BTC, 4855 USD, \nNahum's Portfolio: 5 BTC, 500 USD, \n");
  e.PrintUsersOrders (std::cout);
  oss.str("");
  e.PrintUsersOrders (oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nBuy 2 BTC at 100 USD by Dolson\nDolson's Filled Orders (in chronological order):\nBuy 5 BTC at 100 USD by Dolson\nNahum's Open Orders (in chronological order):\nNahum's Filled Orders (in chronological order):\nSell 5 BTC at 100 USD by Nahum\n");
  std::cout << std::endl << std::endl;
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 3, 100}));
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 7 BTC, 4855 USD, \nNahum's Portfolio: 2 BTC, 700 USD, \n");
  e.PrintUsersOrders (std::cout);
  oss.str("");
  e.PrintUsersOrders (oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nDolson's Filled Orders (in chronological order):\nBuy 5 BTC at 100 USD by Dolson\nBuy 2 BTC at 100 USD by Dolson\nNahum's Open Orders (in chronological order):\nSell 1 BTC at 100 USD by Nahum\nNahum's Filled Orders (in chronological order):\nSell 5 BTC at 100 USD by Nahum\nSell 2 BTC at 100 USD by Nahum\n");
}