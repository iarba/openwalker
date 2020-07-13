#ifndef CONTEXT_H
#define CONTEXT_H

#include "world.h"
#include <iostream>

class context_t
{
public:
  world_t* world = NULL;
  oid_t grid_id = 0;
  oid_t element_id = 0;
};

std::ostream& operator<<(std::ostream &os, const context_t &data);

std::istream& operator>>(std::istream &is, context_t &data);

#endif // CONTEXT_H
