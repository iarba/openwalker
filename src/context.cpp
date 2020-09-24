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
  auto r = world->get_grid(grid_id);
  if(r == NULL)
  {
    throw std::logic_error("Grid not found!");
  }
  return r;
}

cell_t *context_t::cell()
{
  auto r = grid()->at(cell_pos);
  if(r == NULL)
  {
    throw std::logic_error("Cell not found!");
  }
  return r;
}

structure_t *context_t::structure()
{
  auto r = grid()->get_structure(element_id);
  if(r == NULL)
  {
    throw std::logic_error("Structure not found!");
  }
  return r;
}

walker_t *context_t::walker()
{
  auto r = grid()->get_walker(element_id);
  if(r == NULL)
  {
    throw std::logic_error("Walker not found!");
  }
  return r;
}

std::ostream& operator<<(std::ostream &os, const context_t &data)
{
  // world pointer is masqueraded
  os << " " << data.grid_id << " " << data.element_id << " " << data.cell_pos.x << " " << data.cell_pos.y << " " << data.seed << " ";
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
  ow_assert(is >> data.seed);
  return is;
}
