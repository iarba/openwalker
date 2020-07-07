#include "master.h"
#include <chrono>
#include <functional>

master_t::master_t() : world_t()
{
  duty_thread = std::thread(std::bind(&master_t::duty, this));
}

master_t::~master_t()
{
  kill = true;
  duty_thread.join();
}

master_delta master_t::compute_delta()
{
  master_lock.lock();
  context_t ctx;
  ctx.world = this;
  master_delta md;
  md.wd = world_t::compute_delta(ctx);
  master_lock.unlock();
  return md;
}

void master_t::apply_delta(master_delta md)
{
  master_lock.lock();
  for(auto listener : listeners)
  {
    listener->feed(md);
  }
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

void master_t::duty()
{
  while(true)
  {
    master_lock.lock();
    if(kill)
    {
      master_lock.unlock();
      return;
    }
    if(autorun)
    {
      tick();
    }
    master_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}
