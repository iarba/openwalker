#include "context.h"

context_t::context_t()
{
  this->grid = NULL;
}

grid_t *context_t::get_grid()
{
  return this->grid;
}

void context_t::set_grid(grid_t *grid)
{
  this->grid = grid;
}
