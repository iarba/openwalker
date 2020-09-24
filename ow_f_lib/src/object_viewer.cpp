#include "object_viewer.h"

world_viewer_t::world_viewer_t(world_t *base) : world_t()
{
  base->copy_into(this);
}

world_viewer_t::~world_viewer_t()
{
}

grid_viewer_t::grid_viewer_t(grid_t *base) : grid_t(base->get_size())
{
  base->copy_into(this);
}

grid_viewer_t::~grid_viewer_t()
{
}
