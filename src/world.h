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
  bool is_slice_not_triggers = true;
  std::map<oid_t, grid_t *> grid_spawns;
  std::map<oid_t, grid_delta *> grid_deltas;
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
  void set_deletion_queue_usage(bool value);
  void deletion_queue_maintenance();
  dirt_t *dirt;
protected:
  std::map<oid_t, grid_t *> grids;
  bool is_slice_not_triggers = true;
  mutable std::mutex critical_lock;
private:
  bool deletion_queue_mode = false;
  std::vector<grid_t *> grid_deletion_queue;
};

#endif // WORLD_H
