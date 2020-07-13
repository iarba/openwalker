#include "event.h"
#include <map>
#include "misc_utils.h"

std::map<namer_t, std::pair<std::function<double(context_t)>, std::function<void(context_t)>>> registered_events;

double ow_event_helpers::ret_0(context_t ctx)
{
  return 0;
}

double ow_event_helpers::ret_1(context_t ctx)
{
  return 1;
}

void ow_event_helpers::do_nothing(context_t ctx)
{
}

void event_load()
{
  imp_zone(ow_d_events);
  imp(ow_d_events, nop);
  event_register(ow_d_events__nop, ow_event_helpers::ret_0, ow_event_helpers::do_nothing);
}

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

void event_t::serialise(std::ostream &os) const
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
