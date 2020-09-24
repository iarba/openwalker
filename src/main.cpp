#include "main.h"

std::stringstream def_logger;

void openwalker_init(int seed)
{
  def_logger.setstate(std::ios_base::badbit);
  if(!ow_l)
  {
    ow_l = &def_logger;
  }
  srand(seed);
  event_load();

  dirt_t::load();
  master_t::load();
  cloner_t::load();
  grid_t::load();
  structure_t::load();
  walker_t::load();
  xoshirorandomiser::load();
}

void openwalker_set_logger(std::ostream *logger)
{
  ow_l = logger;
}

void openwalker_destroy()
{
  cloner_t::destroy();
  xoshirorandomiser::destroy();
}
