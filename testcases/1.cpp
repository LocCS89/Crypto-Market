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
}
