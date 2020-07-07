#include "structure.h"

structure_t::structure_t(glm::ivec2 position)
{
  this->position = position;
}

structure_t::~structure_t()
{
}

glm::ivec2 structure_t::get_position()
{
  return this->position;
}

structure_delta structure_t::compute_delta(context_t ctx) const
{
  structure_delta sd;
  return sd;
}

void structure_t::append_influence_delta(influence_delta &id, context_t ctx) const
{
}

void structure_t::apply_delta(structure_delta sd)
{
  this->suicide = sd.suicide;
}

bool structure_t::get_suicide()
{
  return this->suicide;
}
