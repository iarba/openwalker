#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "openwalker/openwalker.h"

def(cloner_registry, road_cloner);
def_zone(test);
def(test, walkable);

void tst_load();

class road : public structure_t
{
public:
  class road_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *s);
    virtual structure_t *deserialise(std::istream &is);
  };

  road(glm::ivec2 position);
  road(std::istream &is);
  virtual ~road();
  void serialise(std::ostream &os);
  virtual void copy_into(road *other);
  void append_influence_delta(influence_delta &id, context_t ctx) const;
};

#endif // TEST_TEST_H
