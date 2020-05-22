#include "walker.h"

walker_t::walker_t(glm::dvec2 position, context_t *ctx)
{
  this->position = position;
  this->ctx = ctx;
}

glm::dvec2 walker_t::get_position()
{
  return this->position;
}
