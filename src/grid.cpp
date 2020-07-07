#include "grid.h"

cell_t::cell_t()
{
}

void cell_t::discard()
{
  temporary.clear();
}

void cell_t::set(namer_t name, value_t value)
{
  temporary[name] = value;
}

void cell_t::set_persistent(namer_t name, value_t value)
{
  persistent[name] = value;
}

void cell_t::unset(namer_t name)
{
  temporary.erase(name);
  persistent.erase(name);
}

value_t cell_t::get(namer_t name, value_t def, bool *found)
{
  value_t rval = def;
  bool aux;
  if(found == NULL)
  {
    found = &aux; // we're just gonna pretend it doesn't exist
  }
  *found = false;
  auto it = temporary.find(name);
  if(it != temporary.end())
  {
    *found = true;
    rval = it -> second;
  }
  it = persistent.find(name);
  if(it != persistent.end())
  {
    *found = true;
    rval = it -> second;
  }
  return rval;
}

value_t cell_t::get(namer_t name)
{
  bool found;
  value_t rval = get(name, &found, NULL);
  if(found)
  {
    return rval;
  }
  throw std::logic_error("property not found");
}

grid_t::grid_t(glm::ivec2 size)
{
  this->size = size;
  grid = new cell_t*[size.x];
  for(int i = 0; i < size.x; i++)
  {
    grid[i] = new cell_t[size.y];
  }
}

grid_t::~grid_t()
{
  for(int i = 0; i < this->size.x; i++)
  {
    delete[] grid[i];
  }
  delete[] grid;
  for(auto it : this->structures)
  {
    delete it.second;
  }
  for(auto it : this->walkers)
  {
    delete it.second;
  }
}

glm::ivec2 grid_t::get_size()
{
  return this->size;
}

cell_t *grid_t::at(glm::ivec2 position)
{
  return this->grid[position.x] + position.y;
}

structure_t *grid_t::get_structure(oid_t id)
{
  auto it = structures.find(id);
  if(it != structures.end())
  {
    return it->second;
  }
  return NULL;
}

walker_t *grid_t::get_walker(oid_t id)
{
  auto it = walkers.find(id);
  if(it != walkers.end())
  {
    return it->second;
  }
  return NULL;
}

grid_delta grid_t::compute_delta(context_t ctx) const
{
  grid_delta gd;
  for(auto it : this->structures)
  {
    ctx.element_id = it.first;
    gd.structure_deltas[it.first] = it.second->compute_delta(ctx);
  }
  for(auto it : this->walkers)
  {
    ctx.element_id = it.first;
    gd.walker_deltas[it.first] = it.second->compute_delta(ctx);
  }
  for(auto it : this->structures)
  {
    ctx.element_id = it.first;
    it.second->append_influence_delta(gd.inf_delta, ctx);
  }
  for(auto it : this->walkers)
  {
    ctx.element_id = it.first;
    it.second->append_influence_delta(gd.inf_delta, ctx);
  }
  return gd;
}

void grid_t::apply_delta(grid_delta gd)
{
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      at({i, j})->discard();
    }
  }
  for(auto it : gd.inf_delta.cell_persistent_setters)
  {
    cell_t *c = at(it.first);
    for(auto it2 : it.second)
    {
      c->set_persistent(it2.first, it2.second);
    }
  }
  for(auto it : gd.inf_delta.cell_temporary_setters)
  {
    cell_t *c = at(it.first);
    for(auto it2 : it.second)
    {
      c->set(it2.first, it2.second);
    }
  }
  for(auto it : gd.structure_spawns)
  {
    structures[it.first] = it.second;
  }
  std::map<oid_t, structure_t *> new_structures = structures;
  for(auto it : gd.structure_deltas)
  {
    structures[it.first]->apply_delta(it.second);
    if(structures[it.first]->get_suicide())
    {
      delete structures[it.first];
      new_structures.erase(it.first);
    }
  }
  structures = new_structures;
  for(auto it : gd.walker_spawns)
  {
    walkers[it.first] = it.second;
  }
  std::map<oid_t, walker_t *> new_walkers = walkers;
  for(auto it : gd.walker_deltas)
  {
    walkers[it.first]->apply_delta(it.second);
    if(walkers[it.first]->get_suicide())
    {
      delete walkers[it.first];
      new_walkers.erase(it.first);
    }
  }
  walkers = new_walkers;
  this->suicide = gd.suicide;
}

bool grid_t::get_suicide()
{
  return suicide;
}
