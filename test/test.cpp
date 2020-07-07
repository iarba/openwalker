#include "test.h"

void run()
{
  openwalker_master->tick();
  master_delta md;
  md.wd.grid_spawns[1] = new grid_t({20, 20});
  md.wd.grid_deltas[1].structure_spawns[1] = new road({3, 3});
  md.wd.grid_deltas[1].structure_spawns[2] = new road({3, 4});
  md.wd.grid_deltas[1].structure_spawns[3] = new road({4, 4});
  md.wd.grid_deltas[1].structure_spawns[4] = new road({2, 4});
  openwalker_master->apply_delta(md); // injection
  openwalker_master->tick();
  md.wd.grid_spawns.clear();
  md.wd.grid_deltas[1].structure_spawns.clear();
  md.wd.grid_deltas[1].walker_spawns[1] = new random_walker_t({3, 3}, test_walkable, (value_t)1, (value_t)1, (value_t)0);
  openwalker_master->apply_delta(md); // injection
  openwalker_master->tick();
}

road::road(glm::ivec2 position) : structure_t(position)
{
}

road::~road()
{
}

void road::append_influence_delta(influence_delta &id, context_t ctx) const
{
  structure_t::append_influence_delta(id, ctx);
  id.cell_persistent_setters[position][test_walkable] = (value_t)1;
}

int main()
{
  openwalker_init(1234);
  run();
  return 0;
}
