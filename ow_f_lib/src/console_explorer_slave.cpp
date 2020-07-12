#include "console_explorer_slave.h"
#include <iostream>
#include <vector>

#define EUT(x) std::cerr << "unidentified token: " << x;

console_explorer_slave_t::console_explorer_slave_t(node_t *n) : node_t(n)
{
  ctx.world = this;
  
}

console_explorer_slave_t::~console_explorer_slave_t()
{
}

void console_explorer_slave_t::feed_command(std::string com)
{
  std::vector<std::string> tokens = tokenize(com);
  int current = 0;
  if(tokens[current] == "show")
  {
    current++;
    if(tokens[current] == "context")
    {
      std::cout << "grid: " << ctx.grid_id << (walker_focus ? "walker: " : "structure: ") << ctx.element_id;
      return;
    }
    EUT(tokens[current]);
    return;
  }
}
