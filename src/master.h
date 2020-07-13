#ifndef MASTER_H
#define MASTER_H

#include "node.h"
#include <set>
#include <thread>
#include "properties.h"

def_zone(master_config);
def(master_config, autorun);
def(master_config, delay);
def(master_config, def_user);

class master_t : public node_t
{
public:
  static void load();
  master_t();
  virtual ~master_t();
  void receive_com(command_t c);
  void tick();
  void allow(namer_t usr, namer_t ev_code);
  void update();
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
