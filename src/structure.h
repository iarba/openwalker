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
  virtual ~abstract_structure_constructor_base();
  virtual structure_t *instantiate(structure_t *s) = 0;
  virtual structure_t *deserialise(std::istream &is) = 0;
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

  structure_t(glm::ivec2 position, glm::ivec2 size = {1, 1});
  structure_t(std::istream &is);
  virtual ~structure_t();
  virtual void serialise(std::ostream &os) const;
  void copy_into(structure_t *other) const;
  glm::ivec2 get_position() const;
  bool get_suicide() const;
  glm::ivec2 get_size() const;
  namer_t get_clone_identifier() const;
  void append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx) const;
  void trigger_create(context_t ctx);
  void trigger_delete(context_t ctx);
  instance_event_handler_t ieh;
  namer_t clone_identifier;
  bool suicide = false;
  properties_t properties;
  glm::ivec2 position;
  glm::ivec2 size;
  int ttl = 0;
protected:
private:
};

#endif // STRUCTURE_H
