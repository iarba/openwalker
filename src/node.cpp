#include "node.h"

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

node_delta::~node_delta()
{
  if(wd)
  {
    delete wd;
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
  for(auto listener : listeners)
  {
    listener->feed(nd);
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
  parent->receive_com(c);
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

void node_t::apply_delta(node_delta *nd)
{
  if(nd->wd)
  {
    world_t::apply_delta(nd->wd);
  }
}
