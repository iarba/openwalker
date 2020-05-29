#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "properties.h"
#include <map>
#include "ivec2comparator.h"

class structure_delta
{
public:
  std::map<glm::ivec2, properties_t, ivec2comparator> delta_cell_temporary_setters;
  std::map<glm::ivec2, properties_t, ivec2comparator> delta_cell_persistent_setters;
  properties_t delta_properties;
  bool suicide = false;
};

class structure_t
{
public:
  structure_t(glm::ivec2 position, glm::ivec2 size, context_t *ctx);
  virtual ~structure_t();
  glm::ivec2 get_position();
  glm::ivec2 get_size();
  virtual structure_delta compute_delta() const;
  void apply_delta(structure_delta wd);
  bool get_suicide();
protected:
  bool suicide = false;
  properties_t properties;
  glm::ivec2 position;
  glm::ivec2 size;
  context_t *ctx;
private:
};

#endif // STRUCTURE_H
