#ifndef WALKER_H
#define WALKER_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "properties.h"

typedef double direction_t;

inline direction_t dir_east = 0.0 * M_PI / 4.0;
inline direction_t dir_north_east = 1.0 * M_PI / 4.0;
inline direction_t dir_north = 2.0 * M_PI / 4.0;
inline direction_t dir_north_west = 3.0 * M_PI / 4.0;
inline direction_t dir_west = 4.0 * M_PI / 4.0;
inline direction_t dir_south_west = 5.0 * M_PI / 4.0;
inline direction_t dir_south = 6.0 * M_PI / 4.0;
inline direction_t dir_south_east = 7.0 * M_PI / 4.0;

class walker_delta
{
public:
  direction_t delta_direction = 0;
  double delta_speed = 0;
  glm::dvec2 delta_position = {0, 0};
  bool suicide = false;
  properties_t delta_properties;
};

class walker_t
{
public:
  walker_t(glm::dvec2 position);
  virtual ~walker_t();
  glm::dvec2 get_position();
  bool get_suicide();
  virtual walker_delta compute_delta(context_t ctx) const;
  virtual void append_influence_delta(influence_delta &id, context_t ctx) const;
  void apply_delta(walker_delta wd);
protected:
  properties_t properties;
  bool suicide = false;
  direction_t direction = dir_east;
  double speed = 1;
  glm::dvec2 position;
private:
};

#endif // WALKER_H
