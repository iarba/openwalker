#ifndef RANDOM_WALKER_H
#define RANDOM_WALKER_H

#include "walker.h"

// a random walker moves in 4 directions - N E S W
// a random walker stops when it grid-allignes, then chooses a new direction
// if possible to not turn back, it will continue, otherwise it will turn back

class random_walker_t : public walker_t
{
public:
  static void load();
  class random_walker_constructor : public abstract_walker_constructor_base
  {
    virtual walker_t *instantiate(walker_t *g);
  };

  random_walker_t(glm::dvec2 position, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value);
  virtual ~random_walker_t();
  virtual walker_delta compute_delta(context_t ctx) const;
protected:
  namer_t required_pathfinding_property;
  value_t required_pathfinding_min_value;
  value_t required_pathfinding_max_value;
  value_t required_pathfinding_def_value;
};

#endif // RANDOM_WALKER_H
