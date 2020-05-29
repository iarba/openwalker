#include "test.h"
#include "../src/main.h"

def_zone(test);
def(test, walkable);

test_context *ctx;

void initialise()
{
  ctx = new test_context();
}

void run()
{
  ctx -> run();
}

void terminate()
{
  delete ctx;
}

test_context::test_context() : context_t()
{
  grid_t *g = new grid_t({10, 10}, this);
  this->set_grid(g);
}

test_context::~test_context()
{
}

void test_context::run()
{
  slicer::tick(ctx);
  grid_delta gd;
  gd.structure_spawns[1] = new road({3, 3}, ctx);
  gd.structure_spawns[2] = new road({3, 4}, ctx);
  gd.structure_spawns[3] = new road({4, 4}, ctx);
  gd.structure_spawns[4] = new road({2, 4}, ctx);
  ctx->get_grid()->apply_delta(gd);
  slicer::tick(ctx);
  gd.structure_spawns.clear();
  gd.walker_spawns[1] = new random_walker_t({3, 3}, ctx, test_walkable, (void *)1, (void *)1, (void *)0);
  ctx->get_grid()->apply_delta(gd);
  slicer::tick(ctx);
  slicer::tick(ctx);
}

road::road(glm::ivec2 position, context_t *ctx) : structure_t(position, {1, 1}, ctx)
{
}

road::~road()
{
}

int main()
{
  openwalker_init(1234);
  initialise();
  run();
  terminate();
  return 0;
}
