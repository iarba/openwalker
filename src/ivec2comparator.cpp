#include "ivec2comparator.h"

bool ivec2comparator::operator()(const glm::ivec2 &a, const glm::ivec2 &b) const
{
  if(a.x < b.x)
  {
    return true;
  }
  if(a.y < b.y)
  {
    return true;
  }
  return false;
}
