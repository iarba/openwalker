#include <openwalker/openwalker.h>
#include <ow_f_lib/ow_f_lib.h>
#include "../tstlib/test.h"
#include <sstream>

#define port 7777

def(test, gr1);
def(test, gr42);
def(test, s1);
def(test, s2);
def(test, s3);
def(test, s4);
def(test, w1);

void debug()
{
}

void run(master_t *master)
{
  master->tick();
  node_delta *nd = new node_delta();
  nd->wd = new world_delta();
  nd->wd->grid_deltas[test__gr1] = new grid_delta();

  nd->wd->grid_spawns[test__gr1] = new grid_t({25, 25});  
  nd->wd->grid_deltas[test__gr1]->structure_spawns[test__s1] = new road({3, 3});
  nd->wd->grid_deltas[test__gr1]->structure_spawns[test__s2] = new road({3, 4});
  nd->wd->grid_deltas[test__gr1]->structure_spawns[test__s3] = new road({4, 4});
  nd->wd->grid_deltas[test__gr1]->structure_spawns[test__s4] = new road({2, 4});

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[test__gr1] = new grid_delta();

  master->tick();

  nd->wd->grid_deltas[test__gr1]->walker_spawns[test__w1] = new random_walker_t({3, 3}, test__walkable, (value_t)1, (value_t)1, (value_t)0);

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[test__gr1] = new grid_delta();

  master->tick();

  nd->wd->grid_spawns[test__gr42] = cloner_t::g_cloner_get()->create_grid(master->get_grid(test__gr1));

  master->feed(nd); // injection
  delete nd->wd;
  nd->wd = new world_delta();
  nd->wd->grid_deltas[test__gr1] = new grid_delta();

  master->tick();
  master->tick();
  master->tick();

  if(master->get_grid(test__gr1)->get_structure(test__s4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }  
  if(master->get_grid(test__gr1)->get_structure(test__s4)->get_clone_identifier() != cloner_registry__road_cloner)
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
  imp(test, gr1);
  imp(test, gr42);
  debug();
  server_t *srv = NULL;
  master_t *master = new master_t();
  master->allow(master_config__def_user, test__ev_toggle_pause);
  srv = new server_t(master);
  srv->listen_global(port);
  std::cout << "a test port has been opened on port " << port << "\n";
  getchar();
  console_explorer_slave_t *ces = new console_explorer_slave_t(master);
  run(master);
  if(ces->get_grid(test__gr1)->get_structure(test__s4)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect cloner");
  }
  std::stringstream ios;
  ios.std::iostream::rdbuf(new b_streambuf());
  ces->serialise(ios);
  world_t *dbg = new world_t(ios);
  if(dbg->get_grid(test__gr42)->get_structure(test__s1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect serialiser");
  }
  dbg->serialise(ios);
  delete dbg;
  dbg = new world_t(ios);
  if(dbg->get_grid(test__gr42)->get_structure(test__s1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect series serialiser");
  }
  dbg->serialise(ios);
  delete dbg;
  dbg = new world_t(ios);
  if(dbg->get_grid(test__gr42)->get_structure(test__s1)->get_clone_identifier() != cloner_registry__road_cloner)
  {
    throw std::logic_error("incorrect series serialiser");
  }
  stream_forwarding_node_t *sfn = new stream_forwarding_node_t(ces, &ios);
  sfn->set_forwarding(true);
  master->tick();
  node_delta nd(ios);
  command_t c;
  c.ev_code = test__ev_toggle_pause;
  c.usr = master_config__def_user;
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
