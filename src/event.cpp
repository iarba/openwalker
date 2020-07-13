#include "event.h"
#include <map>
#include "misc_utils.h"

std::map<namer_t, std::pair<std::function<double(context_t)>, std::function<void(context_t)>>> registered_events;

void event_register(namer_t ev_id, std::function<double(context_t)> chance, std::function<void(context_t)> trigger)
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

void event_t::serialise(std::ostream &os)
{
  os << " " << ev_id << " ";
}

double event_t::chance(context_t ctx)
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
