#ifndef DIRT_H
#define DIRT_H

#include "properties.h"
#include <map>
#include <set>
#include <mutex>
#include <functional>

typedef std::function<void(void *)> eraser_t;

void null_eraser(void *v);

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
  value_t get_val(namer_t where);
  void drop_val(namer_t where);
  void set_val(namer_t where, value_t val);
  void *get_dirt(namer_t where);
  void drop_dirt(namer_t where);
  void erase_dirt(namer_t where);
  void set_dirt(namer_t where, void *p, eraser_t eraser = null_eraser);
private:
  std::map<namer_t, value_t> dirt_entries;
  std::map<namer_t, std::pair<void *, eraser_t>> sub_dirt;
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
