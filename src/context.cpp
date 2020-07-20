#include "context.h"
#include "misc_utils.h"

bool context_t::valid()
{
  auto gr = world->get_grid(grid_id);
  if(!gr)
  {
    return false;
  }
  if(cell_pos.x < 0 || cell_pos.y < 0)
  {
    return false;
  }
  glm::ivec2 sz = gr->get_size();
  if(cell_pos.x > sz.x || cell_pos.y > sz.y)
  {
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream &os, const context_t &data)
{
  // world pointer is masqueraded
  os << " " << data.grid_id << " " << data.element_id << " " << data.cell_pos.x << " " << data.cell_pos.y << " ";
  return os;
}

std::istream& operator>>(std::istream &is, context_t &data)
{
  // world pointer is masqueraded
  data.world = NULL;
  ow_assert(is >> data.grid_id);
  ow_assert(is >> data.element_id);
  ow_assert(is >> data.cell_pos.x);
  ow_assert(is >> data.cell_pos.y);
  return is;
}
