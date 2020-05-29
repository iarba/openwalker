#ifndef RANDOM_WALKER_H
#define RANDOM_WALKER_H

#include "walker.h"

// a random walker moves in 4 directions - N E S W
// a random walker stops when it grid-allignes, then chooses a new direction
// if possible to not turn back, it will continue, otherwise it will turn back

class random_walker_t : public walker_t
{
  random_walker_t(glm::dvec2 position, context_t *ctx, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value);
  virtual walker_delta compute_delta() const;
protected:
  namer_t required_pathfinding_property;
  value_t required_pathfinding_min_value;
  value_t required_pathfinding_max_value;
  value_t required_pathfinding_def_value;
};

#endif // RANDOM_WALKER_H
