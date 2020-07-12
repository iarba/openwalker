#ifndef CONSOLE_EXPLORER_SLAVE
#define CONSOLE_EXPLORER_SLAVE

#include <openwalker/openwalker.h>

class console_explorer_slave_t : public node_t
{
public:
  console_explorer_slave_t(node_t *n);
  virtual ~console_explorer_slave_t();
  void feed_command(std::string com);
protected:
  context_t ctx;
  bool walker_focus = false;
};

#endif // CONSOLE_EXPLORER_SLAVE
