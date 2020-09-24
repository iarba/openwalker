#include "event.h"
#include <map>
#include "misc_utils.h"

std::map<namer_t, std::pair<std::function<bool(context_t)>, std::function<void(context_t)>>> registered_events;

bool ow_event_helpers::ret_0(context_t ctx)
{
  return 0;
}

bool ow_event_helpers::never(context_t ctx)
{
  return 0;
}

bool ow_event_helpers::ret_1(context_t ctx)
{
  return 1;
}

bool ow_event_helpers::always(context_t ctx)
{
  return 1;
}

void ow_event_helpers::do_nothing(context_t ctx)
{
}

bool ow_event_helpers::get_grid_suicide(context_t ctx)
{
  return ctx.grid()->get_suicide();
}

void ow_event_helpers::handle_grid_suicide(context_t ctx)
{
  ctx.grid()->trigger_delete(ctx);
  ctx.world->grid_deletion_queue.push_back(ctx.grid());
  ctx.world->grids.erase(ctx.grid_id);
}

bool ow_event_helpers::get_walker_suicide(context_t ctx)
{
  return ctx.walker()->get_suicide();
}

void ow_event_helpers::handle_walker_suicide(context_t ctx)
{
  ctx.walker()->trigger_delete(ctx);
  ctx.world->walker_deletion_queue.push_back(ctx.walker());
  ctx.grid()->walkers.erase(ctx.element_id);
}

bool ow_event_helpers::get_structure_suicide(context_t ctx)
{
  return ctx.structure()->get_suicide();
}

void ow_event_helpers::handle_structure_suicide(context_t ctx)
{
  ctx.structure()->trigger_delete(ctx);
  ctx.world->structure_deletion_queue.push_back(ctx.structure());
  ctx.grid()->structures.erase(ctx.element_id);
}

void ow_event_helpers::handle_aging(context_t ctx)
{
  grid_t *gr = ctx.grid();
  gr->ttl--;
  if(gr->ttl == 0)
  {
    gr->suicide = true;
  }
  for(auto it : gr->walkers)
  {
    it.second->ttl--;
    if(it.second->ttl == 0)
    {
      it.second->suicide = true;
    }
  }
  for(auto it : gr->structures)
  {
    it.second->ttl--;
    if(it.second->ttl == 0)
    {
      it.second->suicide = true;
    }
  }
}

void event_load()
{
  imp_zone(ow_d_events);
  imp(ow_d_events, nop);
  imp(ow_d_events, grid_suicide);
  imp(ow_d_events, walker_suicide);
  imp(ow_d_events, structure_suicide);
  imp(ow_d_events, aging);
  event_register(ow_d_events__nop, ow_event_helpers::ret_0, ow_event_helpers::do_nothing);
  event_register(ow_d_events__grid_suicide, ow_event_helpers::get_grid_suicide, ow_event_helpers::handle_grid_suicide);
  event_register(ow_d_events__walker_suicide, ow_event_helpers::get_walker_suicide, ow_event_helpers::handle_walker_suicide);
  event_register(ow_d_events__structure_suicide, ow_event_helpers::get_structure_suicide, ow_event_helpers::handle_structure_suicide);
  event_register(ow_d_events__aging, ow_event_helpers::always, ow_event_helpers::handle_aging);
}

void event_register(namer_t ev_id, std::function<bool(context_t)> chance, std::function<void(context_t)> trigger)
{
  registered_events[ev_id] = {chance, trigger};
}

event_t::event_t(namer_t ev_id)
{
  this->ev_id = ev_id;
  preload();
}

event_t::event_t(const event_t &other)
{
  this->ev_id = other.ev_id;
  preload();
}

event_t::event_t(std::istream &is)
{
  ow_assert(is >> ev_id);
  preload();
}

event_t::~event_t()
{
}

void event_t::serialise(std::ostream &os) const
{
  os << " " << ev_id << " ";
}

bool event_t::chance(context_t ctx)
{
  return c_chance(ctx);
}

void event_t::trigger(context_t ctx)
{
  return c_trigger(ctx);
}

void event_t::preload()
{
  auto it = registered_events.find(ev_id);
  c_chance = it->second.first;
  c_trigger = it->second.second;
}

std::ostream& operator<<(std::ostream &os, const instance_event_handler_t &data)
{
  os << " " << data.on_create.size() << " ";
  for(auto &event : data.on_create)
  {
    event.serialise(os);
  }
  os << " " << data.on_delete.size() << " ";
  for(auto &event : data.on_delete)
  {
    event.serialise(os);
  }
  os << " " << data.on_random.size() << " ";
  for(auto &event : data.on_random)
  {
    event.serialise(os);
  }
  return os;
}

std::istream& operator>>(std::istream &is, instance_event_handler_t &data)
{
  int count;
  ow_assert(is >> count);
  while(count--)
  {
    data.on_create.push_back(event_t(is));
  }
  ow_assert(is >> count);
  while(count--)
  {
    data.on_delete.push_back(event_t(is));
  }
  ow_assert(is >> count);
  while(count--)
  {
    data.on_random.push_back(event_t(is));
  }
  return is;
}
