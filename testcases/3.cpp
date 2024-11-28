#include "../exchange.hpp"
#include "../useraccount.hpp"
#include "../utility.hpp"

#include <iostream>
#include <sstream>
#include <cassert>


int main() {
  Exchange e;
  e.MakeDeposit("Nahum", "BTC", 10);
  e.PrintUserPortfolios(std::cout);
  std::cout << std::endl;
  assert(!e.MakeWithdrawal("Dolson", "USD", 10)); // Dolson has no portfolio
  assert(!e.MakeWithdrawal("Nahum", "USD", 10)); // Nahum has no USD
  assert(!e.MakeWithdrawal("Nahum", "BTC", 11)); // Nahum doesn't have enough BTC
  assert(e.MakeWithdrawal("Nahum", "BTC", 6)); // Success!
  std::ostringstream oss;
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nNahum's Portfolio: 4 BTC, \n");
  assert(e.MakeWithdrawal("Nahum", "BTC", 4)); 
  // Success! Remove the last of the BTC
  // Please note that you shouldn't report assets that have an amount of 0.
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nNahum's Portfolio: \n");
  e.MakeDeposit("Dolson", "BTC", 4);
  e.MakeDeposit("Dolson", "USD", 4000);
  e.MakeDeposit("Ofria", "ETH", 77);
  assert(e.MakeWithdrawal("Dolson", "BTC", 1)); 
  assert(e.MakeWithdrawal("Dolson", "USD", 4000)); 
  assert(!e.MakeWithdrawal("Ofria", "BTC", 1)); 
  assert(!e.MakeWithdrawal("Ofria", "Apples", 1)); 
  e.PrintUserPortfolios(std::cout);
  oss.str("");
  e.PrintUserPortfolios(oss);
  assert(oss.str() == "User Portfolios (in alphabetical order):\nDolson's Portfolio: 3 BTC, \nNahum's Portfolio: \nOfria's Portfolio: 77 ETH, \n");
}
