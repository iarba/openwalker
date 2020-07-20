#ifndef SPAWN_UTILS_H
#define SPAWN_UTILS_H

#include <openwalker/openwalker.h>
#include <functional>

bool ctx_to_true(context_t _ctx);

std::vector<context_t> get_neighbouring_cells_of_structure(context_t structure, int limit = 0, std::function<bool(context_t)> filter = ctx_to_true);

#endif // SPAWN_UTILS_H
