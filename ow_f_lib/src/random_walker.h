#ifndef RANDOM_WALKER_H
#define RANDOM_WALKER_H

#include <openwalker/openwalker.h>
#include <functional>
#include "ow_f_lib.h"

// a random walker moves in 4 directions - N E S W
// a random walker stops when it grid-allignes, then chooses a new direction
// if possible to not turn back, it will continue, otherwise it will turn back

def(ow_f_lib, event_random_walker_walk);

class random_walker_t : public walker_t
{
public:
  static void load();
  class random_walker_constructor : public abstract_walker_constructor_base
  {
    virtual walker_t *instantiate(walker_t *g);
    virtual walker_t *deserialise(std::istream &is);
  };

  random_walker_t(glm::dvec2 position, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value);
  random_walker_t(glm::dvec2 position, namer_t nvp_namer);
  random_walker_t(std::istream &is);
  virtual ~random_walker_t();
  virtual void serialise(std::ostream &os) const;
  void copy_into(random_walker_t *other) const;
  static void compute_delta(context_t ctx);
  std::function<bool(context_t)> get_pathfinding_checker();
protected:
  void regenerate_pathfinding_checker();
  bool use_nvp = false;
  namer_t pathfinding_nvp = null__null;
  std::function<bool(context_t)> pathfinding_checker;
  namer_t required_pathfinding_property = null__null;
  value_t required_pathfinding_min_value = 0;
  value_t required_pathfinding_max_value = 0;
  value_t required_pathfinding_def_value = 0;
};

#endif // RANDOM_WALKER_H
