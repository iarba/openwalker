#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "properties.h"
#include <map>

class structure_delta
{
public:
  bool suicide = false;
};

class structure_t
{
public:
  structure_t(glm::ivec2 position);
  virtual ~structure_t();
  glm::ivec2 get_position();
  virtual structure_delta compute_delta(context_t ctx) const;
  virtual void append_influence_delta(influence_delta &id, context_t ctx) const;
  void apply_delta(structure_delta wd);
  bool get_suicide();
protected:
  bool suicide = false;
  properties_t properties;
  glm::ivec2 position;
private:
};

#endif // STRUCTURE_H
