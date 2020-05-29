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

value_t cell_t::get(namer_t name)
{
  auto it = temporary.find(name);
  if(it != temporary.end())
  {
    return it -> second;
  }
  it = persistent.find(name);
  if(it != persistent.end())
  {
    return it -> second;
  }
  throw std::logic_error("property not found");
}

value_t cell_t::get(namer_t name, value_t def)
{
  auto it = temporary.find(name);
  if(it != temporary.end())
  {
    return it -> second;
  }
  it = persistent.find(name);
  if(it != persistent.end())
  {
    return it -> second;
  }
  return def;
}

grid_t::grid_t(glm::ivec2 size, context_t *ctx)
{
  this->size = size;
  this->ctx = ctx;
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

grid_delta grid_t::compute_delta() const
{
  grid_delta gd;
  for(auto it : this->structures)
  {
    gd.structure_deltas[it.first] = it.second->compute_delta();
  }
  for(auto it : this->walkers)
  {
    gd.walker_deltas[it.first] = it.second->compute_delta();
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
}
