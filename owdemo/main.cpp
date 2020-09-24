#include "owdemo_main.h"
#include <iostream>
#include "common_prop.h"
#include "road.h"
#include "well.h"
#include "water_carrier.h"

namer_t g_oid = {0, 0};
def_dyn_zone(oid_allocator);

void build_world(context_t ctx)
{
  master_t *m = (master_t *)ctx.world;

  grid_t *gr = new grid_t({100, 100});
  ctx.grid_id = g_oid;
  m->grids[g_oid] = gr;
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new well_t({6, 6});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({7, 8});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({8, 8});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({8, 9});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({8, 10});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({9, 10});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({10, 10});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({10, 9});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({10, 8});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  acquire_dyn(oid_allocator, ctx.element_id);
  gr->structures[ctx.element_id] = new road_t({9, 8});
  gr->structures[ctx.element_id]->trigger_create(ctx);
  int count = 1;
  while(count--)
  {
    acquire_dyn(oid_allocator, ctx.element_id);
    gr->walkers[ctx.element_id] = new water_carrier_t({7, 8});
    gr->walkers[ctx.element_id]->trigger_create(ctx);
  }
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
  imp(owdemo, event_dbg_create);
  imp_dyn_zone(oid_allocator);

  common_prop::init(view);
  road_t::init(view);
  well_t::init(view);
  water_carrier_t::init(view);

  event_register(owdemo__event_dbg_create, ow_event_helpers::ret_0, build_world);
  node_delta nd;
  context_t ctx;
  nd.wd = new world_delta();
  nd.wd->triggers.push_back({event_t(owdemo__event_dbg_create), ctx});
  m->feed(&nd);
  

  view->set(g_oid);
  m->conf(true, 33);

  view->start_render();

  delete view;
  delete man;
  delete m;
  openwalker_destroy();
  return 0;
}
