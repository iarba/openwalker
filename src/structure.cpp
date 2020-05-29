#include "structure.h"

structure_t::structure_t(glm::ivec2 position, glm::ivec2 size, context_t *ctx)
{
  this->position = position;
  this->size = size;
  this->ctx = ctx;
}

structure_t::~structure_t()
{
}

glm::ivec2 structure_t::get_position()
{
  return this->position;
}

glm::ivec2 structure_t::get_size()
{
  return this->size;
}

structure_delta structure_t::compute_delta() const
{
  structure_delta sd;
  return sd;
}

void structure_t::apply_delta(structure_delta sd)
{
  for(auto it : sd.delta_cell_temporary_setters)
  {
    for(auto it2 : it.second)
    {
      this->ctx->get_grid()->at(it.first)->set(it2.first, it2.second);
    }
  }
  for(auto it : sd.delta_cell_persistent_setters)
  {
    for(auto it2 : it.second)
    {
      this->ctx->get_grid()->at(it.first)->set_persistent(it2.first, it2.second);
    }
  }
  for(auto it : sd.delta_properties)
  {
    this->properties[it.first] = it.second;
  }
  this->suicide = sd.suicide;
}

bool structure_t::get_suicide()
{
  return this->suicide;
}
