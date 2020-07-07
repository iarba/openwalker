#ifndef SLAVE_H
#define SLAVE_H

#include "_fwd.h"
#include "master.h"
#include <mutex>

class slave_t : public world_t
{
public:
  slave_t();
  virtual ~slave_t();
  void feed(master_delta md);
protected:
  std::mutex slave_lock;
};

class local_slave_t : public slave_t
{
public:
  local_slave_t(master_t *master);
  virtual ~local_slave_t();
protected:
  master_t *master;
};

#endif // SLAVE_H
