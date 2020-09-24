#ifndef WORLD_H
#define WORLD_H

#include "_fwd.h"
#include "grid.h"
#include "properties.h"
#include "event.h"
#include <iostream>
#include <map>
#include <vector>
#include "dirt.h"

class world_delta
{
public:
  world_delta();
  world_delta(const world_delta *other);
  world_delta(std::istream &is);
  ~world_delta();
  void serialise(std::ostream &os);
  std::vector<std::pair<event_t, context_t>> triggers;
};

class world_t
{
public:
  world_t();
  world_t(std::istream &is);
  virtual ~world_t();
  virtual void serialise(std::ostream &os);
  void copy_into(world_t *other) const;
  grid_t *get_grid(oid_t id);
  world_delta *compute_delta(context_t ctx) const;
  void apply_delta(world_delta *wd);
  std::map<oid_t, grid_t *> get_grids();
  void deletion_queue_maintenance();
  std::map<oid_t, grid_t *> grids;
  mutable std::mutex critical_lock;
  std::vector<grid_t *> grid_deletion_queue;
  std::vector<walker_t *> walker_deletion_queue;
  std::vector<structure_t *> structure_deletion_queue;
protected:
private:
};

#endif // WORLD_H
