#ifndef CONTEXT_H
#define CONTEXT_H

#include "_fwd.h"
#include "world.h"
#include "properties.h"
#include <iostream>

class context_t
{
public:
  bool valid();
  grid_t *grid();
  cell_t *cell();
  structure_t *structure();
  walker_t *walker();
  world_t* world = NULL;
  oid_t grid_id = null__null;
  oid_t element_id = null__null;
  glm::ivec2 cell_pos = {0, 0};
  uint64_t seed = 0;
};

std::ostream& operator<<(std::ostream &os, const context_t &data);

std::istream& operator>>(std::istream &is, context_t &data);

#endif // CONTEXT_H
