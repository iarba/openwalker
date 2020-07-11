#ifndef MASTER_H
#define MASTER_H

#include "node.h"
#include <set>
#include <thread>

class master_t : public node_t
{
public:
  master_t();
  virtual ~master_t();
  void receive_com(command_t c);
  void tick();
protected:
  bool autorun = false;
  int delay = 500;
private:
  node_delta *compute_delta();
  void duty();
  bool kill = false;
  std::thread duty_thread;
};

#endif // MASTER_H
