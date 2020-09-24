#include "walker.h"
#include "misc_utils.h"
#include "xoshirowrapper.h"

abstract_walker_constructor_base::~abstract_walker_constructor_base()
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
  ieh.on_random.push_back(event_t(ow_d_events__walker_suicide));
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
  ow_assert(is >> ttl);
  clone_identifier = cloner_registry__walker_cloner;
}

walker_t::~walker_t()
{
}

void walker_t::serialise(std::ostream &os) const
{
  os << " " << get_clone_identifier() << " "; // only base class is required to do this.
  os << " " << suicide << " ";
  os << ""  << position.x << " " << position.y << " ";
  os << " " << direction << " ";
  os << " " << speed << " ";
  os << " " << properties << " ";
  os << " " << ieh << " ";
  os << " " << ttl << " ";
}

void walker_t::copy_into(walker_t *other) const
{
  other->suicide = this->suicide;
  other->position = this->position;
  other->direction = this->direction;
  other->speed = this->speed;
  other->properties = this->properties;
  other->ieh = this->ieh;
}

glm::dvec2 walker_t::get_position() const
{
  return this->position;
}

bool walker_t::get_suicide() const
{
  return this->suicide;
}

namer_t walker_t::get_clone_identifier() const
{
  return clone_identifier;
}

void walker_t::append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx) const
{
  xoshirorandomiser r(ctx.seed);
  for(auto ev : ieh.on_random)
  {
    ctx.seed = r.next();
    if(ev.chance(ctx))
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
