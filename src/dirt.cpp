#include "dirt.h"

void null_eraser(value_t v)
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
  for(auto it : dirt_entries)
  {
    it.second.second(it.second.first);
  }
  dirt_entries.clear();
}

void erase_dirt_t(value_t p)
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

value_t dirt_t::get(namer_t where)
{
  auto it = dirt_entries.find(where);
  if(it == dirt_entries.end())
  {
    return NULL;
  }
  return it->second.first;
}

void dirt_t::drop(namer_t where)
{
  dirt_entries.erase(where);
}

void dirt_t::erase(namer_t where)
{
  auto it = dirt_entries.find(where);
  if(it != dirt_entries.end())
  {
    it->second.second(it->second.first);
    dirt_entries.erase(it);
  }
}

void dirt_t::set(namer_t where, void* p, eraser_t eraser)
{
  dirt_entries[where] = {p, eraser};
}

dirt_context_t::dirt_context_t() : dirt_t()
{
}

dirt_context_t::~dirt_context_t()
{
}

void erase_dirt_context_t(value_t p)
{
  dirt_context_t *pp = (dirt_context_t *)p;
  delete pp;
}

eraser_t dirt_context_t::get_eraser()
{
  return erase_dirt_context_t;
}
