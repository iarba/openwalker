#ifndef INFLUENCE_H
#define INFLUENCE_H

#include "properties.h"
#include "ivec2comparator.h"

class influence_delta
{
public:
  influence_delta instantiate();
  std::map<glm::ivec2, properties_t, ivec2comparator> cell_persistent_setters;
  std::map<glm::ivec2, properties_t, ivec2comparator> cell_temporary_setters;
  influence_delta operator+(const influence_delta &other);
};

#endif // INFLUENCE_H
