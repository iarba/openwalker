#include <ow_f_lib/ow_f_lib.h>

void ow_f_lib_init()
{
  imp_zone(ow_f_lib);
  spawn_utils::load();
  random_walker_t::load();
  menu_t::load();
}
