#include "walker.h"

walker_delta walker_delta::instantiate()
{
  walker_delta wd;
  wd.delta_direction = this->delta_direction;
  wd.delta_speed = this->delta_speed;
  wd.delta_position = this->delta_position;
  wd.suicide = this->suicide;
  wd.delta_properties = this->delta_properties;
  return wd;
}

def(cloner_registry, walker_cloner);

void walker_t::load()
{
  imp(cloner_registry, walker_cloner);
  cloner_t::g_cloner_get()->reg_walker(cloner_registry__walker_cloner, new walker_constructor());
}

walker_t *walker_t::walker_constructor::instantiate(walker_t *w)
{
  walker_t *nw = new walker_t(w->position);
  w->copy_into(nw);
  return nw;
}

walker_t::walker_t(glm::dvec2 position)
{
  this->position = position;
  clone_identifier = cloner_registry__walker_cloner;
}

walker_t::~walker_t()
{
}

void walker_t::copy_into(walker_t *other)
{
  other->suicide = this->suicide;
  other->position = this->position;
  other->direction = this->direction;
  other->speed = this->speed;
  other->properties = this->properties;
}

glm::dvec2 walker_t::get_position()
{
  return this->position;
}

bool walker_t::get_suicide()
{
  return this->suicide;
}

walker_delta walker_t::compute_delta(context_t ctx) const
{
  walker_delta wd;
  return wd;
}

void walker_t::append_influence_delta(influence_delta &id, context_t ctx) const
{
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

namer_t walker_t::get_clone_identifier()
{
  return clone_identifier;
}
