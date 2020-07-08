#include "master.h"
#include <chrono>
#include <functional>

master_t::master_t() : node_t(NULL)
{
  duty_thread = std::thread(std::bind(&master_t::duty, this));
}

master_t::~master_t()
{
  kill = true;
  duty_thread.join();
}

node_delta *master_t::compute_delta()
{
  node_lock.lock();
  context_t ctx;
  ctx.world = this;
  node_delta *nd = new node_delta();
  nd->wd = world_t::compute_delta(ctx);
  node_lock.unlock();
  return nd;
}

void master_t::tick()
{//not locked!
  node_delta *nd = compute_delta();
  feed(nd);
  delete nd;
}

void master_t::duty()
{
  while(true)
  {
    node_lock.lock();
    if(kill)
    {
      node_lock.unlock();
      return;
    }
    if(autorun)
    {
      node_lock.unlock();
      tick();
      node_lock.lock();
    }
    node_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}
