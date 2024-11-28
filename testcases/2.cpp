#include "../exchange.hpp"
#include "../useraccount.hpp"
#include "../utility.hpp"

#include <iostream>
#include <sstream>
#include <cassert>


int main() {
  Exchange e;
  e.MakeDeposit("Nahum", "BTC", 10);
  e.MakeDeposit("Nahum", "USD", 8500);
  e.MakeDeposit("Dolson", "USD", 600);
  e.MakeDeposit("Nahum", "USD", 500);
  e.MakeDeposit("Dolson", "ETH", 60);
  e.MakeDeposit("Nahum", "ETH", 4);
  e.MakeDeposit("Ofria", "BTC", 100);
  e.PrintUserPortfolios(std::cout);
  e.MakeDeposit("Dolson", "ETH", 10);
  e.PrintUserPortfolios(std::cout);
  std::ostringstream oss;
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 70 ETH, 600 USD, \nNahum's Portfolio: 10 BTC, 4 ETH, 9000 USD, \nOfria's Portfolio: 100 BTC, \n");
}
