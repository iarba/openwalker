#ifndef CLONE_H
#define CLONE_H

#include "_fwd.h"
#include "grid.h"
#include "walker.h"
#include "structure.h"
#include <iostream>

def_zone(cloner_registry);

class cloner_t
{
public:
  static void load();
  static cloner_t *g_cloner_get();
  static void destroy();
  cloner_t();
  ~cloner_t();
  void reg_grid(namer_t where, abstract_grid_constructor_base *agc);
  grid_t *create_grid(grid_t *g);
  grid_t *create_grid(std::istream &is);
  void reg_structure(namer_t where, abstract_structure_constructor_base *asc);
  structure_t *create_structure(structure_t *s);
  structure_t *create_structure(std::istream &is);
  void reg_walker(namer_t where, abstract_walker_constructor_base *awc);
  walker_t *create_walker(walker_t *w);
  walker_t *create_walker(std::istream &is);
private:
  std::map<namer_t, abstract_grid_constructor_base *> grid_constructors;
  std::map<namer_t, abstract_structure_constructor_base *> structure_constructors;
  std::map<namer_t, abstract_walker_constructor_base *> walker_constructors;
};

#endif // CLONE_H
