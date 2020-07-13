#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "properties.h"
#include "event.h"
#include <map>
#include <vector>
#include <functional>

class abstract_structure_constructor_base
{
public:
  virtual structure_t *instantiate(structure_t *s) = 0;
  virtual structure_t *deserialise(std::istream &is) = 0;
};

class structure_delta
{
public:
  structure_delta();
  structure_delta(const structure_delta *other);
  structure_delta(std::istream &is);
  ~structure_delta();
  void serialise(std::ostream &os);
  bool suicide = false;
};

class structure_t
{
public:
  static void load();
  class structure_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *g);
    virtual structure_t *deserialise(std::istream &is);
  };

  structure_t(glm::ivec2 position);
  structure_t(std::istream &is);
  virtual ~structure_t();
  virtual void serialise(std::ostream &os) const;
  virtual void copy_into(structure_t *other) const;
  glm::ivec2 get_position() const;
  virtual structure_delta *compute_delta(context_t ctx) const;
  virtual void append_influence_delta(influence_delta &id, context_t ctx) const;
  void apply_delta(structure_delta *wd);
  bool get_suicide() const;
  namer_t get_clone_identifier() const;
  void append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx, std::function<double()> roll) const;
  void trigger_create(context_t ctx);
  void trigger_delete(context_t ctx);
protected:
  instance_event_handler_t ieh;
  namer_t clone_identifier;
  bool suicide = false;
  properties_t properties;
  glm::ivec2 position;
private:
};

#endif // STRUCTURE_H
