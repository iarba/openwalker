#include "manipulator.h"

manipulator_t::manipulator_t(node_t *n)
{
  this->n = n;
}

manipulator_t::~manipulator_t()
{
}

bool manipulator_t::is_closed()
{
  return closed;
}

void manipulator_t::close()
{
  closed = true;
}
