#include "context.h"
#include "misc_utils.h"

std::ostream& operator<<(std::ostream &os, const context_t &data)
{
  // world pointer is masqueraded
  os << " " << data.grid_id << " " << data.element_id << " ";
  return os;
}

std::istream& operator>>(std::istream &is, context_t &data)
{
  // world pointer is masqueraded
  data.world = NULL;
  ow_assert(is >> data.grid_id);
  ow_assert(is >> data.element_id);
  return is;
}
