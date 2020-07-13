#include "node.h"
#include <functional>
#include "misc_utils.h"

command_t::command_t()
{
}

command_t::command_t(std::istream &is)
{
  ow_assert(is >> usr >> ev_code >> args);
}

command_t::~command_t()
{
}

void command_t::serialise(std::ostream &os)
{
  os << " " << usr << " " << ev_code << " " << args << " ";
}

node_delta::node_delta()
{
}

node_delta::node_delta(const node_delta *other)
{
  if(other->wd)
  {
    this->wd = new world_delta(other->wd);
  }
}

node_delta::node_delta(std::istream &is)
{
  bool wd_present;
  ow_assert(is >> wd_present);
  wd = wd_present? new world_delta(is) : NULL;
}

node_delta::~node_delta()
{
  if(wd)
  {
    delete wd;
  }
}

void node_delta::serialise(std::ostream &os)
{
  if(wd)
  {
    os << " " << true << " ";
    wd->serialise(os);
  }
  else
  {
    os << " " << false << " ";
  }
}

node_t::node_t(node_t *parent)
{
  this->parent = parent;
  if(parent)
  {
    node_lock.lock();
    parent->add_listener(this);
    node_lock.unlock();
  }
}

node_t::~node_t()
{
  if(parent)
  {
    parent->remove_listener(this);
  }
  for(auto listener : listeners)
  {
    listener->forget(this);
  }
}

void node_t::serialise(std::ostream &os)
{
  node_lock.lock();
  this->world_t::serialise(os);
  node_lock.unlock();
}

void node_t::feed(node_delta *nd)
{
  node_lock.lock();
  if(forwarding_enabled)
  {
    for(auto listener : listeners)
    {
      listener->feed(nd);
    }
  }
  if(self_apply)
  {
    apply_delta(nd);
  }
  node_lock.unlock();
}

void node_t::receive_com(command_t c)
{
  node_lock.lock();
  if(parent)
  {
    parent->receive_com(c);
  }
  node_lock.unlock();
}

void node_t::add_listener(node_t *n)
{
  node_lock.lock();
  listeners.insert(n);
  node_lock.unlock();
}

void node_t::remove_listener(node_t *n)
{
  node_lock.lock();
  listeners.erase(n);
  node_lock.unlock();
}

void node_t::set_forwarding(bool value)
{
  forwarding_enabled = value;
}

void node_t::forget(node_t *n)
{
  node_lock.lock();
  if(n == parent)
  {
    parent = NULL;
  }
  node_lock.unlock();
}

void node_t::apply_delta(node_delta *nd)
{
  if(nd->wd)
  {
    world_t::apply_delta(nd->wd);
  }
}

stream_forwarding_node_t::stream_forwarding_node_t(node_t *parent, std::ostream *os, std::istream *is) : node_t(parent)
{
  forwarding_enabled = false;
  this->os = os;
  this->is = is;
  self_apply = false;
  if(is)
  {
    duty_thread = std::thread(std::bind(&stream_forwarding_node_t::duty, this));
  }
}

stream_forwarding_node_t::~stream_forwarding_node_t()
{
  kill = true;
  if(is)
  {
    duty_thread.join();
  }
}

void stream_forwarding_node_t::feed(node_delta *nd)
{
  this->node_t::feed(nd);
  node_lock.lock();
  if(forwarding_enabled)
  {
    nd->serialise(*os);
  }
  node_lock.unlock();
}

void stream_forwarding_node_t::duty()
{
  while(true)
  {
    node_lock.lock();
    if(kill)
    {
      node_lock.unlock();
      return;
    }
    node_lock.unlock();
    ow_safe(command_t c(*is);receive_com(c););
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}

stream_fetching_node_t::stream_fetching_node_t(std::ostream *os, std::istream *is) : node_t(NULL)
{
  this->os = os;
  this->is = is;
  self_apply = false;
  duty_thread = std::thread(std::bind(&stream_fetching_node_t::duty, this));
}

stream_fetching_node_t::~stream_fetching_node_t()
{
  kill = true;
  duty_thread.join();
}

void stream_fetching_node_t::receive_com(command_t c)
{
  this->node_t::receive_com(c); // pretty pointless since master = NULL
  node_lock.lock();
  c.serialise(*os);
  node_lock.unlock();
}

void stream_fetching_node_t::duty()
{
  while(true)
  {
    node_lock.lock();
    if(kill)
    {
      node_lock.unlock();
      return;
    }
    node_lock.unlock();
    ow_safe(node_delta *nd = new node_delta(*is);feed(nd);delete nd;);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}
