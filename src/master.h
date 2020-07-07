#ifndef MASTER_H
#define MASTER_H

#include "_fwd.h"
#include "world.h"
#include "slave.h"
#include <mutex>
#include <set>
#include <thread>

class master_delta;
class master_t;

struct command_t
{
  int opcode;
};

class master_delta
{
public:
  world_delta wd;
};

class master_t : public world_t
{
public:
  master_t();
  virtual ~master_t();
  master_delta compute_delta();
  void apply_delta(master_delta md);
  void receive_com(command_t c);
  void add_listener(slave_t *s);
  void remove_listener(slave_t *s);
  void tick();
protected:
  std::mutex master_lock;
  std::set<slave_t *>listeners;
  bool autorun = false;
  int delay = 1;
private:
  void duty();
  bool kill = false;
  std::thread duty_thread;
};

#endif // MASTER_H
