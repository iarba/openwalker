#ifndef INFLUENCE_H
#define INFLUENCE_H

#include "properties.h"
#include "ivec2comparator.h"
#include <iostream>

class influence_delta
{
public:
  influence_delta();
  influence_delta(std::istream &is);
  void serialise(std::ostream &os);
  std::map<glm::ivec2, properties_t, ivec2comparator> cell_persistent_setters;
  std::map<glm::ivec2, properties_t, ivec2comparator> cell_temporary_setters;
  std::map<glm::ivec2, properties_t, ivec2comparator> cell_unsetters;
  influence_delta operator+(const influence_delta &other);
};

#endif // INFLUENCE_H
