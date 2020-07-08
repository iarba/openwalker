#include "clone.h"

cloner_t *g_cloner = NULL;

void cloner_t::load()
{
  if(g_cloner == NULL)
  {
    g_cloner = new cloner_t();
  }
  imp_zone(cloner_registry);
}

cloner_t *cloner_t::g_cloner_get()
{
  return g_cloner;
}

void cloner_t::destroy()
{
  if(g_cloner)
  {
    delete g_cloner;
  }
  g_cloner = NULL;
}

cloner_t::cloner_t()
{
}

cloner_t::~cloner_t()
{
  for(auto it : grid_constructors)
  {
    delete it.second;
  }
  for(auto it : structure_constructors)
  {
    delete it.second;
  }
  for(auto it : walker_constructors)
  {
    delete it.second;
  }
}

void cloner_t::reg_grid(namer_t where, abstract_grid_constructor_base *agc)
{
  grid_constructors[where] = agc;
}

grid_t *cloner_t::create_grid(grid_t *g)
{
  return grid_constructors[g->get_clone_identifier()]->instantiate(g);
}

grid_t *cloner_t::create_grid(std::istream &is)
{
  namer_t identifier;
  is >> identifier;
  return grid_constructors[identifier]->deserialise(is);
}

void cloner_t::reg_structure(namer_t where, abstract_structure_constructor_base *asc)
{
  structure_constructors[where] = asc;
}

structure_t *cloner_t::create_structure(structure_t *s)
{
  return structure_constructors[s->get_clone_identifier()]->instantiate(s);
}

structure_t *cloner_t::create_structure(std::istream &is)
{
  namer_t identifier;
  is >> identifier;
  return structure_constructors[identifier]->deserialise(is);
}

void cloner_t::reg_walker(namer_t where, abstract_walker_constructor_base *awc)
{
  walker_constructors[where] = awc;
}

walker_t *cloner_t::create_walker(walker_t *w)
{
  return walker_constructors[w->get_clone_identifier()]->instantiate(w);
}

walker_t *cloner_t::create_walker(std::istream &is)
{
  namer_t identifier;
  is >> identifier;
  return walker_constructors[identifier]->deserialise(is);
}
