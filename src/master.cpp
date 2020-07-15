#include "master.h"
#include <chrono>
#include <functional>
#include "misc_utils.h"

void master_t::load()
{
  imp_zone(master_config);
  imp(master_config, autorun);
  imp(master_config, delay);
  imp(master_config, def_user);
}

master_t::master_t() : node_t(NULL)
{
  dirt = new dirt_t();
  duty_thread = std::thread(std::bind(&master_t::duty, this));
  dirt_t *h_index = new dirt_t();
  dirt->set(dirt_index__homes, (value_t) h_index, dirt_t::get_eraser());
  h_index->set(master_config__def_user, (value_t) new dirt_context_t(), dirt_context_t::get_eraser());
  dirt->set(master_config__autorun, (value_t) false);
  dirt->set(master_config__delay, (value_t) 500);
  this->update();
}

master_t::~master_t()
{
  kill = true;
  duty_thread.join();
  delete dirt;
}

void master_t::receive_com(command_t c)
{
  node_lock.lock();
  namer_t ev_code = ow_d_events__nop;
  dirt_t *homes_dirt = (dirt_t *)dirt->get(dirt_index__homes);
  if(homes_dirt)
  {
    dirt_context_t *context_dirt = (dirt_context_t *)homes_dirt->get(c.usr);
    if(context_dirt)
    {
      dirt->set(dirt_index__context, (value_t) context_dirt);
      if(context_dirt->allowed_event_ids.find(c.ev_code) != context_dirt->allowed_event_ids.end())
      {
        ev_code = c.ev_code;
        for(auto it : c.args)
        {
          context_dirt->set(it.first, it.second);
        }
      }
    }
  }
  node_lock.unlock();
  dirt->lock();
  context_t ctx;
  ctx.world = this;
  event_t event(ev_code);
  ow_safe_cont(event.trigger(ctx));
  dirt->unlock();
  dirt->drop(dirt_index__context);
  this->node_t::receive_com(c);
}

void master_t::allow(namer_t usr, namer_t ev_code)
{
  dirt->lock();
  dirt_t *homes_dirt = (dirt_t *)dirt->get(dirt_index__homes);
  if(homes_dirt)
  {
    dirt_context_t *context_dirt = (dirt_context_t *)homes_dirt->get(usr);
    if(context_dirt)
    {
      context_dirt->allowed_event_ids.insert(ev_code);
    }
  }
  dirt->unlock();
}

void master_t::update()
{
  node_lock.lock();
  autorun = (bool) dirt->get(master_config__autorun);
  delay = (long) dirt->get(master_config__delay);
  node_lock.unlock();
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
