#include <ow_f_lib/ow_f_lib.h>
#include <iostream>

// In this directory i will help you make an idea on how to develop efficiently using openwalker.
// We're gonna implement game of life. 
// We're gonna create a wrapping 50x50 map, but we'll abstract the size within the code

// First thing first - we create cell class - see cell.h and cell.cpp. return once you finished
#include "cell.h"

// Now we're gonna do the same thing, but for grid - it will do most of the heavy lifting during the crank
#include "cranking_grid.h"

int main(int argc, char **argv)
{
  // first we configure openwalker
  openwalker_set_logger(&std::cout);
  openwalker_init(1234);
  // then we configure the openwalker free library
  ow_f_lib_init();
  // we need to call our loader functions
  cell_load();
  grid_load();
  // aight awesome, now let's create a world and insert a glider into it
  // we use master since that class alters the contained world.
  master_t *m = new master_t();
  // oh no, the world is empty, doesn't even contain grids. And we don't have a nice way to fix that
  // unless... we could feed a delta into master
  node_delta *nd = new node_delta();
  world_delta *wd = new world_delta();
  nd->wd = wd;
  // we need a new grid
  glm::ivec2 size = {10, 10};
  oid_t g_oid = {1, 0};
  wd->grid_spawns[g_oid] = new cranking_grid(size);
  // and within the grid, we need some cells
  wd->grid_deltas[g_oid] = new grid_delta();
  wd->grid_deltas[g_oid]->structure_spawns[oid_of({5, 5})] = new cell({5, 5});
  wd->grid_deltas[g_oid]->structure_spawns[oid_of({5, 6})] = new cell({5, 6});
  wd->grid_deltas[g_oid]->structure_spawns[oid_of({5, 7})] = new cell({5, 7});
  wd->grid_deltas[g_oid]->structure_spawns[oid_of({6, 7})] = new cell({6, 7});
  wd->grid_deltas[g_oid]->structure_spawns[oid_of({7, 6})] = new cell({7, 6});
  //
  //   ***
  //     *
  //    *
  //
  // and now, we feed the delta to the master
  m->feed(nd);
  // now if we access data in the world, we can see that it's there
  ow_assert(m->get_grid(g_oid)->get_structure(oid_of({5, 5})));
  ow_assert(m->get_grid(g_oid)->get_structure(oid_of({4, 6})) == NULL);
  // let's run a couple of cranks
  m->tick();
  m->tick();
  m->tick();
  m->tick();
  m->tick();
  m->tick();
  m->tick();
  m->tick();
  // at this point we should notice that a previously inexistent cell at 4, 6 now exists
  ow_assert(m->get_grid(g_oid)->get_structure(oid_of({5, 5})) == NULL);
  ow_assert(m->get_grid(g_oid)->get_structure(oid_of({4, 6})));
  // ait let's create a viewer for our game, and enjoy it for a few seconds
  manipulator_t *man = new manipulator_t(m);
  sfml_viewer_t *v = (new sfml_viewer_t(m, man))->set(g_oid);
  m->conf(true, 200);
  v->start_render();
  // clean up after ourselves
  delete v;
  delete man;
  delete nd;
  delete m;
  // and clean openwalker
  openwalker_destroy();
  return 0;
}
