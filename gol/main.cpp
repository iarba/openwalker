#include <ow_f_lib/ow_f_lib.h>
#include <iostream>

// In this directory i will help you make an idea on how to develop efficiently using openwalker.
// We're gonna implement game of life. 
// The crank is gonna be split into 2 rounds.
// - First we update the grid using deltas.
// - Next, we use an event to parse the entire grid and place/delete cells based on rules.
// - We're gonna create a wrapping 50x50 map

// First thing first - we create cell class - see cell.h and cell.cpp. return once you finished


int main(int argc, char **argv)
{
  openwalker_set_logger(&std::cout);
  openwalker_init(1234);
  openwalker_destroy();
  return 0;
}
