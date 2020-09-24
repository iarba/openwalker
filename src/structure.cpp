#include "structure.h"
#include "misc_utils.h"
#include "xoshirowrapper.h"

abstract_structure_constructor_base::~abstract_structure_constructor_base()
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
  structure_t *ns = new structure_t(s->position, s->size);
  s->copy_into(ns);
  return ns;
}

structure_t *structure_t::structure_constructor::deserialise(std::istream &is)
{
  structure_t *ns = new structure_t(is);
  return ns;
}

structure_t::structure_t(glm::ivec2 position, glm::ivec2 size)
{
  this->position = position;
  this->size = size;
  ieh.on_random.push_back(event_t(ow_d_events__structure_suicide));
  clone_identifier = cloner_registry__structure_cloner;
}

structure_t::structure_t(std::istream &is)
{
  ow_assert(is >> position.x >> position.y);
  ow_assert(is >> size.x >> size.y);
  ow_assert(is >> properties);
  ow_assert(is >> suicide);
  ow_assert(is >> ieh);
  clone_identifier = cloner_registry__structure_cloner;
}

structure_t::~structure_t()
{
}

void structure_t::serialise(std::ostream &os) const
{
  os << " " << get_clone_identifier() << " "; // only base class is required to do this.
  os << " " << position.x << " " << position.y << " ";
  os << " " << size.x << " " << size.y << " ";
  os << " " << properties << " ";
  os << " " << suicide << " ";
  os << " " << ieh << " ";
}

void structure_t::copy_into(structure_t *other) const
{
  other->suicide = this->suicide;
  other->position = this->position;
  other->properties = this->properties;
  other->ieh = this->ieh;
}

glm::ivec2 structure_t::get_position() const
{
  return this->position;
}

bool structure_t::get_suicide() const
{
  return this->suicide;
}

glm::ivec2 structure_t::get_size() const
{
  return this->size;
}

namer_t structure_t::get_clone_identifier() const
{
  return clone_identifier;
}

void structure_t::append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx) const
{
  xoshirorandomiser r(ctx.seed);
  for(auto ev : ieh.on_random)
  {
    ctx.seed = r.next();
    if(ev.chance(ctx))
    triggers.push_back({ev, ctx});
  }
}

void structure_t::trigger_create(context_t ctx)
{
  for(auto &event : ieh.on_create)
  {
    event.trigger(ctx);
  }
}

void structure_t::trigger_delete(context_t ctx)
{
  for(auto &event : ieh.on_delete)
  {
    event.trigger(ctx);
  }
}
