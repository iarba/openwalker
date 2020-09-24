#include "master.h"
#include <chrono>
#include <functional>
#include "misc_utils.h"
#include "xoshirowrapper.h"

static void printFPS()
{
  static std::chrono::time_point<std::chrono::system_clock> oldTime = std::chrono::high_resolution_clock::now();
  static int fps;
  fps++;
  if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{1})
  {
    oldTime = std::chrono::high_resolution_clock::now();
    std::cout << "M_FPS: " << fps <<  "\n";
    fps = 0;
  }
}

void master_t::load()
{
  imp_zone(master_config);
  imp(master_config, autorun);
  imp(master_config, delay);
  imp(master_config, def_user);
}

master_t::master_t() : node_t(NULL)
{
  duty_thread = std::thread(std::bind(&master_t::duty, this));
}

master_t::~master_t()
{
  kill = true;
  duty_thread.join();
}

void master_t::receive_com(command_t c)
{
  node_lock.lock();
  namer_t ev_code = ow_d_events__nop;
  context_t ctx;
  ctx.world = this;
  event_t event(ev_code);
  ow_safe_cont(event.trigger(ctx));
  node_lock.unlock();
  this->node_t::receive_com(c);
}

void master_t::conf(bool autorun, int delay)
{
  node_lock.lock();
  this->autorun = autorun;
  this->delay = delay;
  node_lock.unlock();
}

void master_t::toggle_pause()
{
  this->autorun = !this->autorun;
}

node_delta *master_t::compute_delta()
{
  node_lock.lock();
  context_t ctx;
  ctx.seed = xoshirorandomiser::g_rand_get()->next();
  ctx.world = this;
  node_delta *nd = new node_delta();
  nd->wd = world_t::compute_delta(ctx);
  node_lock.unlock();
  printFPS();
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
