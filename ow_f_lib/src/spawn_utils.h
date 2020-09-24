#ifndef SPAWN_UTILS_H
#define SPAWN_UTILS_H

#include <openwalker/openwalker.h>
#include <functional>
#include <vector>

class spawn_utils
{
public:
  static void load();
  static bool ctx_to_true(context_t _ctx);
  static namer_t attach_to(world_t *w, grid_t *g);
  static namer_t attach_to(grid_t *g, walker_t *w);
  static namer_t attach_to(grid_t *g, structure_t *s);
  static std::vector<glm::ivec2> get_neighbouring_cells_of_structure(context_t structure, unsigned int limit = 0, std::function<bool(context_t)> filter = ctx_to_true);
};

#endif // SPAWN_UTILS_H
