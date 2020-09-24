#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "structure.h"
#include "walker.h"
#include "context.h"
#include "properties.h"
#include "influence.h"
#include "clone.h"
#include "event.h"
#include <vector>
#include <functional>
#include <mutex>

class abstract_grid_constructor_base
{
public:
  virtual ~abstract_grid_constructor_base();
  virtual grid_t *instantiate(grid_t *g) = 0;
  virtual grid_t *deserialise(std::istream &is) = 0;
};

class cell_t
{
public:
  cell_t();
  cell_t(std::istream& is);
  virtual ~cell_t();
  void serialise(std::ostream& os);
  virtual void copy_into(cell_t *other);
  void discard();
  void set(namer_t name, value_t value);
  void set_persistent(namer_t name, value_t value);
  void unset(namer_t name);
  value_t get(namer_t name, value_t def, bool *found, properties_t *delta_persistent_setter, properties_t *delta_temporary_setter);
  value_t get(namer_t name, value_t def, bool *found);
  value_t get(namer_t name);
private:
  properties_t temporary;
  properties_t persistent;
};

class grid_t
{
public:
  static void load();
  class grid_constructor : public abstract_grid_constructor_base
  {
    virtual grid_t *instantiate(grid_t *g);
    virtual grid_t *deserialise(std::istream &is);
  };

  grid_t(glm::ivec2 size);
  grid_t(std::istream& is);
  virtual ~grid_t();
  virtual void serialise(std::ostream& os) const;
  void copy_into(grid_t *other) const;
  glm::ivec2 get_size() const;
  cell_t *at(glm::ivec2 position) const;
  structure_t *get_structure(oid_t id) const;
  walker_t *get_walker(oid_t id) const;
  bool get_suicide() const;
  namer_t get_clone_identifier() const;
  void append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx) const;
  void trigger_create(context_t ctx);
  void trigger_delete(context_t ctx);
  std::map<oid_t, structure_t *> get_structures();
  std::map<oid_t, walker_t *> get_walkers();
  instance_event_handler_t ieh;
  namer_t clone_identifier;
  std::map<oid_t, structure_t *> structures;
  std::map<oid_t, walker_t *> walkers;
  glm::ivec2 size;
  cell_t **grid;
  bool suicide = false;
  mutable std::mutex critical_lock;
  int ttl = 0;
protected:
private:
};

#endif // GRID_H
