#include "grid.h"

grid_t::grid_t(glm::ivec2 size, context_t *ctx)
{
  this->size = size;
  this->ctx = ctx;
}

glm::ivec2 grid_t::get_size()
{
  return this->size;
}
