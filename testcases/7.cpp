#include "../exchange.hpp"
#include "../useraccount.hpp"
#include "../utility.hpp"

#include <iostream>
#include <sstream>
#include <cassert>


int main() {
  Exchange e;
  std::ostringstream oss;
  e.MakeDeposit("Nahum", "BTC", 10);
  e.MakeDeposit("Nahum", "USD", 500);
  e.MakeDeposit("Dolson", "USD", 1000);
  e.MakeDeposit("Ofria", "ETH", 44);
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 1000 USD, \nNahum's Portfolio: 10 BTC, 500 USD, \nOfria's Portfolio: 44 ETH, \n");
  e.PrintUsersOrders (std::cout);
  oss.str("");
  e.PrintUsersOrders (oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nDolson's Filled Orders (in chronological order):\nNahum's Open Orders (in chronological order):\nNahum's Filled Orders (in chronological order):\nOfria's Open Orders (in chronological order):\nOfria's Filled Orders (in chronological order):\n");
  assert(!e.AddOrder({"Nahum", "Sell", "BTC", 50, 1000}));
  // Can't place order because Nahum doesn't have enough BTC
  assert(!e.AddOrder({"Nahum", "Buy", "BTC", 50, 200}));
  // Can't place order because Nahum doesn't have enough USD
  assert(e.AddOrder({"Nahum", "Sell", "BTC", 4, 1000}));
  assert(e.AddOrder({"Nahum", "Buy", "ETH", 3, 2}));
  assert(e.AddOrder({"Dolson", "Buy", "BTC", 7, 1}));
  assert(e.AddOrder({"Ofria", "Sell", "ETH", 2, 500}));
  // Make sure to remove orders from portfolios
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 993 USD, \nNahum's Portfolio: 6 BTC, 494 USD, \nOfria's Portfolio: 42 ETH, \n");
  e.PrintUsersOrders(std::cout);
  oss.str("");
  e.PrintUsersOrders (oss);
  assert(oss.str() == "Users Orders (in alphabetical order):\nDolson's Open Orders (in chronological order):\nBuy 7 BTC at 1 USD by Dolson\nDolson's Filled Orders (in chronological order):\nNahum's Open Orders (in chronological order):\nSell 4 BTC at 1000 USD by Nahum\nBuy 3 ETH at 2 USD by Nahum\nNahum's Filled Orders (in chronological order):\nOfria's Open Orders (in chronological order):\nSell 2 ETH at 500 USD by Ofria\nOfria's Filled Orders (in chronological order):\n");
}
