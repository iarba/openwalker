#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "../src/main.h"

def_zone(test);
def(test, walkable);

class road : public structure_t
{
public:
  class road_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *s);
  };

  road(glm::ivec2 position);
  void append_influence_delta(influence_delta &id, context_t ctx) const;
  ~road();
};

#endif // TEST_TEST_H
