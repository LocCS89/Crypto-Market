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
  assert(o1 == o2);
  Order o3 = o2;
  assert(o1 == o3);
  o3.side = "Sell";
  bool result = o1 == o3;
  assert(!result);
}
