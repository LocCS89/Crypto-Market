#include "../exchange.hpp"
#include "../useraccount.hpp"
#include "../utility.hpp"

#include <iostream>
#include <sstream>
#include <cassert>


int main() {
   // This test case includes utility.hpp instead of exchange.cpp
  Order o1 {"Nahum", "Buy", "BTC", 100, 60000};
  Order o2(o1);
  std::ostringstream oss;
  oss << o1;
  assert(oss.str() == "Buy 100 BTC at 60000 USD by Nahum");
  oss.str("");
  oss << Order{"Dolson", "Sell", "ETH", 9, 888};
  assert(oss.str() == "Sell 9 ETH at 888 USD by Dolson");
}
