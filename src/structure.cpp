#include "structure.h"

structure_delta::structure_delta()
{
}

structure_delta::structure_delta(const structure_delta *other)
{
  this->suicide = other->suicide;
}

structure_delta::~structure_delta()
{
}

def(cloner_registry, structure_cloner);

void structure_t::load()
{
  imp(cloner_registry, structure_cloner);
  cloner_t::g_cloner_get()->reg_structure(cloner_registry__structure_cloner, new structure_constructor());
}

structure_t *structure_t::structure_constructor::instantiate(structure_t *s)
{
  structure_t *ns = new structure_t(s->position);
  s->copy_into(ns);
  return ns;
}

structure_t *structure_t::structure_constructor::deserialise(std::istream &is)
{
  structure_t *ns = new structure_t(is);
  return ns;
}

structure_t::structure_t(glm::ivec2 position)
{
  this->position = position;
  clone_identifier = cloner_registry__structure_cloner;
}

structure_t::structure_t(std::istream &is)
{
  is >> position.x >> position.y;
  is >> properties;
  is >> suicide;
  clone_identifier = cloner_registry__structure_cloner;
}

structure_t::~structure_t()
{
}

void structure_t::serialise(std::ostream &os)
{
  os << " " << get_clone_identifier() << " "; // only base class is required to do this.
  os << " " << position.x << " " << position.y << " ";
  os << " " << properties << " ";
  os << " " << suicide << " ";
}

void structure_t::copy_into(structure_t *other)
{
  other->suicide = this->suicide;
  other->position = this->position;
  other->properties = this->properties;
}

glm::ivec2 structure_t::get_position()
{
  return this->position;
}

structure_delta *structure_t::compute_delta(context_t ctx) const
{
  structure_delta *sd = new structure_delta();
  return sd;
}

void structure_t::append_influence_delta(influence_delta &id, context_t ctx) const
{
}

void structure_t::apply_delta(structure_delta *sd)
{
  this->suicide = sd->suicide;
}

bool structure_t::get_suicide()
{
  return this->suicide;
}

namer_t structure_t::get_clone_identifier()
{
  return clone_identifier;
}
