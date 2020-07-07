#include "main.h"

master_t *openwalker_master = NULL;

void openwalker_init(int seed)
{
  srand(seed);
  openwalker_master = new master_t();
}

void openwalker_destroy()
{
  if(openwalker_master)
  {
    delete openwalker_master;
    openwalker_master = NULL;
  }
}
