#include "master.h"

master_t::master_t() : world_t()
{
}

master_t::~master_t()
{
}

master_delta master_t::compute_delta()
{
  master_lock.lock();
  context_t ctx;
  ctx.world = this;
  master_delta md;
  md.wd = world_t::compute_delta(ctx);
  for(auto listener : listeners)
  {
    listener->feed(md);
  }
  master_lock.unlock();
  return md;
}

void master_t::apply_delta(master_delta md)
{
  master_lock.lock();
  world_t::apply_delta(md.wd);
  master_lock.unlock();
}

void master_t::receive_com(command_t c)
{
  master_lock.lock();
  master_lock.unlock();
}

void master_t::add_listener(slave_t *s)
{
  master_lock.lock();
  listeners.insert(s);
  master_lock.unlock();
}

void master_t::remove_listener(slave_t *s)
{
  master_lock.lock();
  listeners.erase(s);
  master_lock.unlock();
}

void master_t::tick()
{
  apply_delta(compute_delta());
}
