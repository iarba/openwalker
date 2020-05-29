#include "walker.h"

walker_t::walker_t(glm::dvec2 position, context_t *ctx)
{
  this->position = position;
  this->ctx = ctx;
}

walker_t::~walker_t()
{
}

glm::dvec2 walker_t::get_position()
{
  return this->position;
}

bool walker_t::get_suicide()
{
  return this->suicide;
}

walker_delta walker_t::compute_delta() const
{
  walker_delta wd;
  return wd;
}

void walker_t::apply_delta(walker_delta wd)
{
  this->direction +=wd.delta_direction;
  this->speed +=wd.delta_speed;
  this->position += wd.delta_position;
  this->suicide = wd.suicide;
  for(auto it : wd.delta_properties)
  {
    this->properties[it.first] = it.second;
  }
}
