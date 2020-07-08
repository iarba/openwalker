#include "walker.h"

walker_delta::walker_delta()
{
}

walker_delta::walker_delta(const walker_delta *other)
{
  walker_delta wd;
  this->delta_direction = other->delta_direction;
  this->delta_speed = other->delta_speed;
  this->delta_position = other->delta_position;
  this->suicide = other->suicide;
  this->delta_properties = other->delta_properties;
}

walker_delta::~walker_delta()
{
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

walker_t *walker_t::walker_constructor::deserialise(std::istream &is)
{
  walker_t *nw = new walker_t(is);
  return nw;
}

walker_t::walker_t(glm::dvec2 position)
{
  this->position = position;
  clone_identifier = cloner_registry__walker_cloner;
}

walker_t::walker_t(std::istream &is)
{
  is >> suicide;
  is >> position.x >> position.y;
  is >> direction;
  is >> speed;
  is >> properties;
  clone_identifier = cloner_registry__walker_cloner;
}

walker_t::~walker_t()
{
}

void walker_t::serialise(std::ostream &os)
{
  os << " " << get_clone_identifier() << " "; // only base class is required to do this.
  os << " " << suicide << " ";
  os << ""  << position.x << " " << position.y << " ";
  os << " " << direction << " ";
  os << " " << speed << " ";
  os << " " << properties << " ";
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

walker_delta *walker_t::compute_delta(context_t ctx) const
{
  walker_delta *wd = new walker_delta();
  return wd;
}

void walker_t::append_influence_delta(influence_delta &id, context_t ctx) const
{
}

void walker_t::apply_delta(walker_delta *wd)
{
  this->direction +=wd->delta_direction;
  this->speed +=wd->delta_speed;
  this->position += wd->delta_position;
  this->suicide = wd->suicide;
  for(auto it : wd->delta_properties)
  {
    this->properties[it.first] = it.second;
  }
}

namer_t walker_t::get_clone_identifier()
{
  return clone_identifier;
}
