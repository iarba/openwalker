#ifndef EVENT_H
#define EVENT_H

#include "_fwd.h"
#include "context.h"
#include <iostream>
#include <functional>

void event_register(namer_t ev_id, std::function<double(context_t ctx)> chance, std::function<void(context_t ctx)> trigger);

//an event is an ideally pure function that is executed at well defined times
class event_t
{
public:
  event_t(namer_t ev_id);
  event_t(const event_t &other);
  event_t(std::istream &is);
  ~event_t();
  void serialise(std::ostream &os);
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
