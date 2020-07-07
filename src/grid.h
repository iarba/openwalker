#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "structure.h"
#include "walker.h"
#include "context.h"
#include "properties.h"
#include "influence.h"

class grid_delta
{
public:
  std::map<oid_t, structure_t *> structure_spawns;
  std::map<oid_t, structure_delta> structure_deltas;
  std::map<oid_t, walker_t *> walker_spawns;
  std::map<oid_t, walker_delta> walker_deltas;
  influence_delta inf_delta;
  bool suicide = false;
};

class cell_t
{
public:
  cell_t();
  void discard();
  void set(namer_t name, value_t value);
  void set_persistent(namer_t name, value_t value);
  void unset(namer_t name);
  value_t get(namer_t name, value_t def, bool *found);
  value_t get(namer_t name);
private:
  properties_t temporary;
  properties_t persistent;
};

class grid_t
{
public:
  grid_t(glm::ivec2 size);
  ~grid_t();
  glm::ivec2 get_size();
  cell_t *at(glm::ivec2 position);
  structure_t *get_structure(oid_t id);
  walker_t *get_walker(oid_t id);
  grid_delta compute_delta(context_t ctx) const;
  void apply_delta(grid_delta gd);
  bool get_suicide();
private:
  std::map<oid_t, structure_t *> structures;
  std::map<oid_t, walker_t *> walkers;
  glm::ivec2 size;
  cell_t **grid;
  bool suicide = false;
};

#endif // GRID_H
