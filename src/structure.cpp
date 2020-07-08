#include "structure.h"

structure_delta structure_delta::instantiate()
{
  structure_delta sd;
  sd.suicide = this->suicide;
  return sd;
}

def(cloner_registry, structure_cloner);

void structure_t::load()
{
  imp(cloner_registry, structure_cloner);
  cloner_t::g_cloner_get()->reg_structure(cloner_registry__structure_cloner, new structure_constructor());
}

structure_t *structure_t::structure_constructor::instantiate(structure_t *g)
{
  structure_t *ns = new structure_t(g->position);
  return ns;
}

structure_t::structure_t(glm::ivec2 position)
{
  this->position = position;
  clone_identifier = cloner_registry__structure_cloner;
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

namer_t structure_t::get_clone_identifier()
{
  return clone_identifier;
}
