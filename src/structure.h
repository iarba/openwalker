#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <glm/glm.hpp>
#include "_fwd.h"
#include "context.h"
#include "properties.h"
#include <map>

class structure_delta
{
public:
  std::map<glm::ivec2, properties_t> delta_cell_temporary_setters;
  std::map<glm::ivec2, properties_t> delta_cell_persistent_setters;
  properties_t delta_properties;
};

class structure_t
{
public:
  structure_t(glm::ivec2 position, glm::ivec2 size, context_t *ctx);
  glm::ivec2 get_position();
  glm::ivec2 get_size();
  virtual structure_delta compute_delta() const;
  void apply_delta(structure_delta wd);
protected:
  properties_t properties;
  bool suicide = false;
  glm::ivec2 position;
  glm::ivec2 size;
  context_t *ctx;
private:
};

#endif // STRUCTURE_H
