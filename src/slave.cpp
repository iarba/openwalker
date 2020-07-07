#include "slave.h"

slave_t::slave_t() : world_t()
{
}

slave_t::~slave_t()
{
}

void slave_t::feed(master_delta md)
{
  slave_lock.lock();
  apply_delta(md.wd);
  slave_lock.unlock();
}

local_slave_t::local_slave_t(master_t *master) : slave_t()
{
  this -> master = master;
  slave_lock.lock();
  master -> add_listener(this);
  slave_lock.unlock();
}

local_slave_t::~local_slave_t()
{
  master -> remove_listener(this);
}
