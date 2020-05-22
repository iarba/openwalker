#ifndef WALKER_H
#define WALKER_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"

class walker_t
{
public:
  walker_t(glm::dvec2 position, context_t *ctx);
  glm::dvec2 get_position();
private:
  glm::dvec2 position;
  context_t *ctx;
};

#endif // WALKER_H
