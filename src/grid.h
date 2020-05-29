#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "structure.h"
#include "walker.h"
#include "context.h"
#include "properties.h"

typedef int oid_t;

class grid_delta
{
public:
  std::map<oid_t, structure_t *> structure_spawns;
  std::map<oid_t, structure_delta> structure_deltas;
  std::map<oid_t, walker_t *> walker_spawns;
  std::map<oid_t, walker_delta> walker_deltas;
};

class cell_t
{
public:
  cell_t();
  void discard();
  void set(namer_t name, value_t value);
  void set_persistent(namer_t name, value_t value);
  void unset(namer_t name);
  value_t get(namer_t name);
  value_t get(namer_t name, value_t def);
private:
  properties_t temporary;
  properties_t persistent;
};

class grid_t
{
public:
  grid_t(glm::ivec2 size, context_t *ctx);
  ~grid_t();
  glm::ivec2 get_size();
  cell_t *at(glm::ivec2 position);
  grid_delta compute_delta() const;
  void apply_delta(grid_delta gd);
private:
  std::map<oid_t, structure_t *> structures;
  std::map<oid_t, walker_t *> walkers;
  glm::ivec2 size;
  context_t *ctx;
  cell_t **grid;
};

#endif // GRID_H
