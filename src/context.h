#ifndef CONTEXT_H
#define CONTEXT_H

#include "world.h"

class context_t
{
public:
  world_t* world = NULL;
  oid_t grid_id = 0;
  oid_t element_id = 0;
};

#endif // CONTEXT_H
