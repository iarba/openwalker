#ifndef CONSOLE_EXPLORER_SLAVE
#define CONSOLE_EXPLORER_SLAVE

#include "slave.h"

class console_explorer_slave_t : public local_slave_t
{
public:
  console_explorer_slave_t(master_t *m);
  virtual ~console_explorer_slave_t();
  void feed_command(std::string com);
protected:
  context_t ctx;
  bool walker_focus = false;
};

#endif // CONSOLE_EXPLORER_SLAVE
