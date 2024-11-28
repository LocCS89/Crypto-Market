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
}
