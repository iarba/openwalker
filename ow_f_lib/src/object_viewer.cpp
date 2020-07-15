#include "object_viewer.h"

world_viewer_t::world_viewer_t(world_t *base) : world_t()
{
  base->copy_into(this);
}

world_viewer_t::~world_viewer_t()
{
}

std::map<oid_t, grid_t *> &world_viewer_t::get_grids()
{
  return grids;
}

bool &world_viewer_t::get_is_slice_not_triggers()
{
  return is_slice_not_triggers;
}

grid_viewer_t::grid_viewer_t(grid_t *base) : grid_t(base->get_size())
{
  base->copy_into(this);
}

grid_viewer_t::~grid_viewer_t()
{
}

std::map<oid_t, structure_t *> &grid_viewer_t::get_structures()
{
  return structures;
}

std::map<oid_t, walker_t *> &grid_viewer_t::get_walkers()
{
  return walkers;
}
