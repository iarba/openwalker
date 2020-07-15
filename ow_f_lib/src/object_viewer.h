#ifndef OBJECT_VIEWER_H
#define OBJECT_VIEWER_H

#include <openwalker/openwalker.h>

class world_viewer_t : public world_t
{
public:
  world_viewer_t(world_t *base);
  virtual ~world_viewer_t();
  std::map<oid_t, grid_t *> &get_grids();
  bool &get_is_slice_not_triggers();
};

class grid_viewer_t : public grid_t
{
public:
  grid_viewer_t(grid_t *base);
  virtual ~grid_viewer_t();
  std::map<oid_t, structure_t *> &get_structures();
  std::map<oid_t, walker_t *> &get_walkers();
};

#endif // OBJECT_VIEWER_H
