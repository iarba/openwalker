#include "owdemo_main.h"
#include <iostream>
#include "common_prop.h"
#include "road.h"
#include "well.h"
#include "water_carrier.h"

namer_t g_oid = {0, 0};
def_dyn_zone(oid_allocator);

master_t *build_world(master_t *m)
{
  oid_t oid;

  node_delta *nd = new node_delta();
  nd->wd = new world_delta();
  nd->wd->grid_spawns[g_oid] = new grid_t({100, 100});
  nd->wd->grid_deltas[g_oid] = new grid_delta();
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new well_t({6, 6});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({7, 8});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({8, 8});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({8, 9});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({8, 10});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({9, 10});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({10, 10});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({10, 9});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({10, 8});
  acquire_dyn(oid_allocator, oid);
  nd->wd->grid_deltas[g_oid]->structure_spawns[oid] = new road_t({9, 8});
  int count = 1;
  while(count--)
  {
    acquire_dyn(oid_allocator, oid);
    nd->wd->grid_deltas[g_oid]->walker_spawns[oid] = new water_carrier_t({7, 8});
  }

  m->feed(nd);
  
  delete nd;
  return m;
}

int main(int argc, char **argv)
{
  openwalker_set_logger(&std::cout);
  openwalker_init(1234);
  ow_f_lib_init();

  master_t *m = new master_t();
  manipulator_t *man = new manipulator_t(m);
  sfml_viewer_t *view = (new sfml_viewer_t(m, man));

  imp_zone(owdemo);
  imp_dyn_zone(oid_allocator);

  common_prop::init(view);
  road_t::init(view);
  well_t::init(view);
  water_carrier_t::init(view);

  build_world(m);

  view->set(g_oid);
  m->conf(true, 33);

  view->start_render();

  delete view;
  delete man;
  delete m;
  openwalker_destroy();
  return 0;
}
