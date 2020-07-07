#ifndef WORLD_H
#define WORLD_H

#include "_fwd.h"
#include "grid.h"
#include "properties.h"

class world_delta
{
public:
  std::map<oid_t, grid_t *> grid_spawns;
  std::map<oid_t, grid_delta> grid_deltas;
};

class world_t
{
public:
  world_t();
  virtual ~world_t();
  grid_t *get_grid(oid_t id);
  world_delta compute_delta(context_t ctx) const;
  void apply_delta(world_delta wd);
private:
  std::map<oid_t, grid_t *> grids;
};

#endif // WORLD_H
