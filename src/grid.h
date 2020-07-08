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

class abstract_grid_constructor_base
{
public:
  virtual grid_t *instantiate(grid_t *g) = 0;
  virtual grid_t *deserialise(std::istream &is) = 0;
};

class grid_delta
{
public:
  grid_delta();
  grid_delta(const grid_delta *other);
  ~grid_delta();
  std::map<oid_t, structure_t *> structure_spawns;
  std::map<oid_t, structure_delta*> structure_deltas;
  std::map<oid_t, walker_t *> walker_spawns;
  std::map<oid_t, walker_delta*> walker_deltas;
  influence_delta inf_delta;
  bool suicide = false;
};

class cell_t
{
public:
  cell_t();
  cell_t(std::istream& is);
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
  ~grid_t();
  virtual void serialise(std::ostream& os);
  void copy_into(grid_t *other);
  glm::ivec2 get_size();
  cell_t *at(glm::ivec2 position);
  structure_t *get_structure(oid_t id);
  walker_t *get_walker(oid_t id);
  grid_delta *compute_delta(context_t ctx) const;
  void apply_delta(grid_delta *gd);
  bool get_suicide();
  namer_t get_clone_identifier();
protected:
  namer_t clone_identifier;
private:
  std::map<oid_t, structure_t *> structures;
  std::map<oid_t, walker_t *> walkers;
  glm::ivec2 size;
  cell_t **grid;
  bool suicide = false;
};

#endif // GRID_H
