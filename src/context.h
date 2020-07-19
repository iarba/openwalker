#ifndef CONTEXT_H
#define CONTEXT_H

#include "world.h"
#include "properties.h"
#include <iostream>

class context_t
{
public:
  world_t* world = NULL;
  oid_t grid_id = null__null;
  oid_t element_id = null__null;
  glm::ivec2 cell_pos = {0, 0};
};

std::ostream& operator<<(std::ostream &os, const context_t &data);

std::istream& operator>>(std::istream &is, context_t &data);

#endif // CONTEXT_H
