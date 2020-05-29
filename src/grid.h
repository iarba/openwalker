#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "properties.h"

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
  glm::ivec2 get_size();
  cell_t *at(glm::ivec2 position);
private:
  glm::ivec2 size;
  context_t *ctx;
  cell_t **grid;
};

#endif // GRID_H
