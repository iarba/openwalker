#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "properties.h"
#include <map>

class abstract_structure_constructor_base
{
public:
  virtual structure_t *instantiate(structure_t *s) = 0;
};

class structure_delta
{
public:
  structure_delta instantiate();
  bool suicide = false;
};

class structure_t
{
public:
  static void load();
  class structure_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *g);
  };

  structure_t(glm::ivec2 position);
  virtual ~structure_t();
  virtual void copy_into(structure_t *other);
  glm::ivec2 get_position();
  virtual structure_delta compute_delta(context_t ctx) const;
  virtual void append_influence_delta(influence_delta &id, context_t ctx) const;
  void apply_delta(structure_delta wd);
  bool get_suicide();
  namer_t get_clone_identifier();
protected:
  namer_t clone_identifier;
  bool suicide = false;
  properties_t properties;
  glm::ivec2 position;
private:
};

#endif // STRUCTURE_H
