#include "structure.h"

structure_t::structure_t(glm::ivec2 position, glm::ivec2 size, context_t *ctx)
{
  this->position = position;
  this->size = size;
  this->ctx = ctx;
}

glm::ivec2 structure_t::get_position()
{
  return this->position;
}

glm::ivec2 structure_t::get_size()
{
  return this->size;
}
