#ifndef IVEC2COMPARATOR_H
#define IVEC2COMPARATOR_H

#include <glm/glm.hpp>

struct ivec2comparator
{
  bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const;
};

#endif // IVEC2COMPARATOR_H
