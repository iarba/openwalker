#include "dirt.h"

void null_eraser(void *_p)
{
}

void dirt_t::load()
{
  imp_zone(dirt_index);
  imp(dirt_index, context);
  imp(dirt_index, homes);
}

dirt_t::dirt_t()
{
}

dirt_t::~dirt_t()
{
  for(auto it : sub_dirt)
  {
    it.second.second(it.second.first);
  }
  sub_dirt.clear();
  dirt_entries.clear();
}

void erase_dirt_t(void *p)
{
  dirt_t *pp = (dirt_t *)p;
  delete pp;
}

eraser_t dirt_t::get_eraser()
{
  return erase_dirt_t;
}

void dirt_t::lock()
{
  dirt_lock.lock();
  locked = true;
}

void dirt_t::unlock()
{
  locked = false;
  dirt_lock.unlock();
}

value_t dirt_t::get_val(namer_t where)
{
  auto it = dirt_entries.find(where);
  if(it == dirt_entries.end())
  {
    return 0;
  }
  return it->second;
}

void dirt_t::drop_val(namer_t where)
{
  dirt_entries.erase(where);
}

void dirt_t::set_val(namer_t where, value_t val)
{
  dirt_entries[where] = val;
}

void *dirt_t::get_dirt(namer_t where)
{
  auto it = sub_dirt.find(where);
  if(it == sub_dirt.end())
  {
    return NULL;
  }
  return it->second.first;
}

void dirt_t::drop_dirt(namer_t where)
{
  sub_dirt.erase(where);
}

void dirt_t::erase_dirt(namer_t where)
{
  auto it = sub_dirt.find(where);
  if(it != sub_dirt.end())
  {
    it->second.second(it->second.first);
    sub_dirt.erase(it);
  }
}


void dirt_t::set_dirt(namer_t where, void *p, eraser_t eraser)
{
  sub_dirt[where] = {p, eraser};
}

dirt_context_t::dirt_context_t() : dirt_t()
{
}

dirt_context_t::~dirt_context_t()
{
}

void erase_dirt_context_t(void *p)
{
  dirt_context_t *pp = (dirt_context_t *)p;
  delete pp;
}

eraser_t dirt_context_t::get_eraser()
{
  return erase_dirt_context_t;
}
