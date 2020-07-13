#include "walker.h"
#include "misc_utils.h"

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

walker_delta::walker_delta(std::istream &is)
{
  ow_assert(is >> delta_direction);
  ow_assert(is >> delta_speed);
  ow_assert(is >> delta_position.x >> delta_position.y);
  ow_assert(is >> suicide);
  ow_assert(is >> delta_properties);
}

walker_delta::~walker_delta()
{
}

void walker_delta::serialise(std::ostream &os)
{
  os << " " << delta_direction << " ";
  os << " " << delta_speed << " ";
  os << " " << delta_position.x << " " << delta_position.y << " ";
  os << " " << suicide << " ";
  os << " " << delta_properties << " ";
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
  ow_assert(is >> suicide);
  ow_assert(is >> position.x >> position.y);
  ow_assert(is >> direction);
  ow_assert(is >> speed);
  ow_assert(is >> properties);
  ow_assert(is >> ieh);
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
  os << " " << ieh << " ";
}

void walker_t::copy_into(walker_t *other)
{
  other->suicide = this->suicide;
  other->position = this->position;
  other->direction = this->direction;
  other->speed = this->speed;
  other->properties = this->properties;
  other->ieh = this->ieh;
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

void walker_t::append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx, std::function<double()> roll)
{
  for(auto ev : ieh.on_random)
  {
    if(ev.chance(ctx) > roll())
    triggers.push_back({ev, ctx});
  }
}

void walker_t::trigger_create(context_t ctx)
{
  for(auto &event : ieh.on_create)
  {
    event.trigger(ctx);
  }
}

void walker_t::trigger_delete(context_t ctx)
{
  for(auto &event : ieh.on_delete)
  {
    event.trigger(ctx);
  }
}
