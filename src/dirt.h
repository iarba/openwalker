#ifndef DIRT_H
#define DIRT_H

#include "properties.h"
#include <map>
#include <set>
#include <mutex>
#include <functional>

typedef std::function<void(void *)> eraser_t;

def_zone(dirt_index);
def(dirt_index, context);
def(dirt_index, homes);


class dirt_t
{
public:
  static void load();
  dirt_t();
  virtual ~dirt_t();
  virtual eraser_t get_eraser();
  static void erase(void *p);
  void lock();
  void unlock();
  void *get(namer_t where);
  void drop(namer_t where);
  void erase(namer_t where);
  void set(namer_t where, void* p, eraser_t eraser);
private:
  std::map<namer_t, std::pair<void *, eraser_t>> dirt_entries;
  std::mutex dirt_lock;
  bool locked = false;
};

class dirt_context_t : public dirt_t
{
public:
  dirt_context_t();
  virtual ~dirt_context_t();
  virtual eraser_t get_eraser();
  //static void erase(void *p);
  namer_t user_id;
  std::set<namer_t> allowed_event_ids;
};

#endif // DIRT_H
