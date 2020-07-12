#include <openwalker/openwalker.h>
#include <ow_f_lib/ow_f_lib.h>
#include "../tstlib/test.h"
#include <sstream>

#define port 7777

void run(master_t *master)
{
  master->tick();
  node_delta *nd = new node_delta();
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  nd->wd->grid_spawns[1] = new grid_t({20, 20});  
  nd->wd->grid_deltas[1]->structure_spawns[1] = new road({3, 3});
  nd->wd->grid_deltas[1]->structure_spawns[2] = new road({3, 4});
  nd->wd->grid_deltas[1]->structure_spawns[3] = new road({4, 4});
  nd->wd->grid_deltas[1]->structure_spawns[4] = new road({2, 4});

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  master->tick();

  nd->wd->grid_deltas[1]->walker_spawns[1] = new random_walker_t({3, 3}, test__walkable, (value_t)1, (value_t)1, (value_t)0);

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  master->tick();

  nd->wd->grid_spawns[42] = cloner_t::g_cloner_get()->create_grid(master->get_grid(1));

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[1] = new grid_delta();

  master->tick();
  master->tick();
  master->tick();

  if(master->get_grid(1)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }  
  if(master->get_grid(42)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }
  delete nd;
}

int main()
{
  openwalker_set_logger(&std::cout);
  openwalker_init(1234);
  ow_f_lib_init();
  tst_load();
  server_t *srv = NULL;
  master_t *master = new master_t();
  srv = new server_t(master);
  srv->listen_global(port);
  std::cout << "a test port has been opened on port " << port << "\n";
  getchar();
  console_explorer_slave_t *ces = new console_explorer_slave_t(master);
  run(master);
  if(ces->get_grid(1)->get_structure(4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }
  std::stringstream ios;
  ios.std::iostream::rdbuf(new b_streambuf());
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
  sfn->set_forwarding(true);
  master->tick();
  node_delta nd(ios);
  command_t c;
  c.opcode=OW_CMD_PAUSE;
  master->receive_com(c);
  delete sfn;
  std::cout << "testing finished, master is unpaused, press enter to terminate\n";
  getchar();
  delete dbg;
  delete ces;
  delete ios.std::iostream::rdbuf();
  if(srv)
  {
    delete srv;
  }
  delete master;
  openwalker_destroy();
  return 0;
}
