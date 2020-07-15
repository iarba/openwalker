#ifndef WALKER_H
#define WALKER_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "properties.h"
#include "event.h"
#include <vector>
#include <functional>

typedef double direction_t;

inline direction_t dir_east = 0.0 * M_PI / 4.0;
inline direction_t dir_north_east = 1.0 * M_PI / 4.0;
inline direction_t dir_north = 2.0 * M_PI / 4.0;
inline direction_t dir_north_west = 3.0 * M_PI / 4.0;
inline direction_t dir_west = 4.0 * M_PI / 4.0;
inline direction_t dir_south_west = 5.0 * M_PI / 4.0;
inline direction_t dir_south = 6.0 * M_PI / 4.0;
inline direction_t dir_south_east = 7.0 * M_PI / 4.0;

class abstract_walker_constructor_base
{
public:
  virtual walker_t *instantiate(walker_t *w) = 0;
  virtual walker_t *deserialise(std::istream &is) = 0;
};

class walker_delta
{
public:
  walker_delta();
  walker_delta(const walker_delta *other);
  walker_delta(std::istream &is);
  ~walker_delta();
  void serialise(std::ostream &os);
  direction_t delta_direction = 0;
  double delta_speed = 0;
  glm::dvec2 delta_position = {0, 0};
  bool suicide = false;
  properties_t delta_properties;
};

class walker_t
{
public:
  static void load();
  class walker_constructor : public abstract_walker_constructor_base
  {
    virtual walker_t *instantiate(walker_t *g);
    virtual walker_t *deserialise(std::istream &is);
  };

  walker_t(glm::dvec2 position);
  walker_t(std::istream &is);
  virtual ~walker_t();
  virtual void serialise(std::ostream &os) const;
  void copy_into(walker_t *other) const;
  glm::dvec2 get_position() const;
  bool get_suicide() const;
  virtual walker_delta *compute_delta(context_t ctx) const;
  virtual void append_influence_delta(influence_delta &id, context_t ctx) const;
  void apply_delta(walker_delta *wd);
  namer_t get_clone_identifier() const;
  void append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx, std::function<double()> roll);
  void trigger_create(context_t ctx);
  void trigger_delete(context_t ctx);
protected:
  instance_event_handler_t ieh;
  namer_t clone_identifier;
  properties_t properties;
  bool suicide = false;
  direction_t direction = dir_east;
  double speed = 1;
  glm::dvec2 position;
private:
};

#endif // WALKER_H
