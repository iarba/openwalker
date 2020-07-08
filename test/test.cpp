#include "test.h"
#include <sstream>

def(cloner_registry, road_cloner);

void load()
{
  imp_zone(test);
  imp(test, walkable);
  imp(cloner_registry, road_cloner);
  cloner_t::g_cloner_get()->reg_structure(cloner_registry__road_cloner, new road::road_constructor());
}

structure_t *road::road_constructor::instantiate(structure_t *s)
{
  road *r = (road *)s;
  road *nr = new road(r->position);
  r->copy_into(nr);
  return nr;
}

structure_t *road::road_constructor::deserialise(std::istream &is)
{
  road *nr = new road(is);
  return nr;
}

void run()
{
  openwalker_master->tick();
  node_delta *nd = new node_delta();
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  nd->wd->grid_spawns[1] = new grid_t({20, 20});  
  nd->wd->grid_deltas[1]->structure_spawns[1] = new road({3, 3});
  nd->wd->grid_deltas[1]->structure_spawns[2] = new road({3, 4});
  nd->wd->grid_deltas[1]->structure_spawns[3] = new road({4, 4});
  nd->wd->grid_deltas[1]->structure_spawns[4] = new road({2, 4});

  openwalker_master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  openwalker_master->tick();

  nd->wd->grid_deltas[1]->walker_spawns[1] = new random_walker_t({3, 3}, test__walkable, (value_t)1, (value_t)1, (value_t)0);

  openwalker_master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  openwalker_master->tick();

  nd->wd->grid_spawns[42] = cloner_t::g_cloner_get()->create_grid(openwalker_master->get_grid(1));

  openwalker_master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  openwalker_master->tick();
  openwalker_master->tick();
  openwalker_master->tick();

  if(openwalker_master->get_grid(1)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }  
  if(openwalker_master->get_grid(42)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }
  delete nd;
}

road::road(glm::ivec2 position) : structure_t(position)
{
  clone_identifier = cloner_registry__road_cloner;
}

road::road(std::istream &is) : structure_t(is)
{
  clone_identifier = cloner_registry__road_cloner;
}

road::~road()
{
}

void road::serialise(std::ostream &os)
{
  this->structure_t::serialise(os);
}

void road::copy_into(road *other)
{
  this->structure_t::copy_into(other);
}

void road::append_influence_delta(influence_delta &id, context_t ctx) const
{
  structure_t::append_influence_delta(id, ctx);
  id.cell_persistent_setters[position][test__walkable] = (value_t)1;
}

int main()
{
  openwalker_init(1234);
  load();
  console_explorer_slave_t *ces = new console_explorer_slave_t(openwalker_master);
  run();
  if(ces->get_grid(1)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }
  std::stringstream ios;
  ces->serialise(ios);
  world_t *dbg = new world_t(ios);
  if(dbg->get_grid(42)->get_structure(1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect serialiser");
  }
  dbg->serialise(ios);
  delete dbg;
  dbg = new world_t(ios);
  if(dbg->get_grid(42)->get_structure(1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect series serialiser");
  }
  dbg->serialise(ios);
  delete dbg;
  dbg = new world_t(ios);
  if(dbg->get_grid(42)->get_structure(1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect series serialiser");
  }
  stream_forwarding_node_t *sfn = new stream_forwarding_node_t(ces, &ios);
  openwalker_master->tick();
  node_delta nd(ios);
  delete sfn;
  delete dbg;
  delete ces;
  openwalker_destroy();
  return 0;
}
