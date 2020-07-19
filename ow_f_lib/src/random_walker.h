#ifndef RANDOM_WALKER_H
#define RANDOM_WALKER_H

#include <openwalker/openwalker.h>
#include <functional>

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
    virtual walker_t *deserialise(std::istream &is);
  };

  random_walker_t(glm::dvec2 position, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value);
  random_walker_t(glm::dvec2 position, namer_t nvp_namer);
  random_walker_t(std::istream &is);
  virtual ~random_walker_t();
  virtual void serialise(std::ostream &os) const;
  void copy_into(random_walker_t *other) const;
  virtual walker_delta *compute_delta(context_t ctx) const;
protected:
  void regenerate_pathfinding_checker();
  bool use_nvp;
  namer_t pathfinding_nvp;
  std::function<bool(context_t)> pathfinding_checker;
  namer_t required_pathfinding_property;
  value_t required_pathfinding_min_value;
  value_t required_pathfinding_max_value;
  value_t required_pathfinding_def_value;
};

#endif // RANDOM_WALKER_H
