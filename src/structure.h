#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"

class structure_t
{
public:
  structure_t(glm::ivec2 position, glm::ivec2 size, context_t *ctx);
  glm::ivec2 get_position();
  glm::ivec2 get_size();
private:
  glm::ivec2 position;
  glm::ivec2 size;
  context_t *ctx;
};

#endif // STRUCTURE_H
