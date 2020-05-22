#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"

class grid_t
{
public:
  grid_t(glm::ivec2 size, context_t *ctx);
  glm::ivec2 get_size();
private:
  glm::ivec2 size;
  context_t *ctx;
};

#endif // GRID_H
