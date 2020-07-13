#ifndef EVENT_H
#define EVENT_H

#include "_fwd.h"
#include "context.h"
#include <iostream>
#include <functional>
#include <vector>

def_zone(ow_d_events);
def(ow_d_events, nop);

namespace ow_event_helpers
{
  double ret_0(context_t ctx);
  double ret_1(context_t ctx);
  void do_nothing(context_t ctx);
};

void event_load();

void event_register(namer_t ev_id, std::function<double(context_t ctx)> chance, std::function<void(context_t ctx)> trigger);

//an event is an ideally pure function that is executed at well defined times
class event_t
{
public:
  event_t(namer_t ev_id);
  event_t(const event_t &other);
  event_t(std::istream &is);
  ~event_t();
  void serialise(std::ostream &os) const;
// pseudo-virtual functions:
  double chance(context_t ctx);
  void trigger(context_t ctx);
private:
  void preload();
  namer_t ev_id;
  std::function<double(context_t)> c_chance;
  std::function<void(context_t)> c_trigger;
};

#endif // EVENT_H

#ifndef INSTANCE_EVENT_HANDLER
#define INSTANCE_EVENT_HANDLER
#include <vector>

class instance_event_handler_t
{
public:
  std::vector<event_t> on_create;
  std::vector<event_t> on_delete;
  std::vector<event_t> on_random;
};

std::ostream& operator<<(std::ostream &os, const instance_event_handler_t &data);

std::istream& operator>>(std::istream &is, instance_event_handler_t &data);

#endif // INSTANCE_EVENT_HANDLER
