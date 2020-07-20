#include "context.h"
#include "misc_utils.h"

bool context_t::valid()
{
  auto gr = grid();
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

grid_t *context_t::grid()
{
  return world->get_grid(grid_id);
}

cell_t *context_t::cell()
{
  return grid()->at(cell_pos);
}

structure_t *context_t::structure()
{
  return grid()->get_structure(element_id);
}

walker_t *context_t::walker()
{
  return grid()->get_walker(element_id);
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
