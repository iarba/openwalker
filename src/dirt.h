#ifndef DIRT_H
#define DIRT_H

#include "properties.h"
#include <map>
#include <set>
#include <mutex>
#include <functional>

typedef std::function<void(value_t)> eraser_t;

void null_eraser(value_t v);

def_zone(dirt_index);
def(dirt_index, context);
def(dirt_index, homes);

class dirt_t
{
public:
  static void load();
  dirt_t();
  virtual ~dirt_t();
  static eraser_t get_eraser();
  void lock();
  void unlock();
  value_t get(namer_t where);
  void drop(namer_t where);
  void erase(namer_t where);
  void set(namer_t where, value_t p, eraser_t eraser = null_eraser);
private:
  std::map<namer_t, std::pair<value_t, eraser_t>> dirt_entries;
  std::mutex dirt_lock;
  bool locked = false;
};

class dirt_context_t : public dirt_t
{
public:
  dirt_context_t();
  virtual ~dirt_context_t();
  static eraser_t get_eraser();
  namer_t user_id;
  std::set<namer_t> allowed_event_ids;
};

#endif // DIRT_H
