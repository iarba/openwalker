#include "world.h"
#include "clone.h"
#include "misc_utils.h"

world_delta::world_delta()
{
}

world_delta::world_delta(const world_delta *other)
{
  for(auto it : other->grid_spawns)
  {
    this->grid_spawns[it.first] = cloner_t::g_cloner_get()->create_grid(it.second);
  }
  for(auto it : other->grid_deltas)
  {
    this->grid_deltas[it.first] = new grid_delta(it.second);
  }
}

world_delta::world_delta(std::istream &is)
{
  int count;
  ow_assert(is >> count);
  while(count--)
  {
    oid_t where;
    ow_assert(is >> where);
    grid_spawns[where] = cloner_t::g_cloner_get()->create_grid(is);
  }
  ow_assert(is >> count);
  while(count--)
  {
    oid_t where;
    ow_assert(is >> where);
    grid_deltas[where] = new grid_delta(is);
  }
}

world_delta::~world_delta()
{
  for(auto it : this->grid_spawns)
  {
    delete it.second;
  }
  this->grid_spawns.clear();
  for(auto it : this->grid_deltas)
  {
    delete it.second;
  }
  this->grid_deltas.clear();
}

void world_delta::serialise(std::ostream &os)
{
  os << " " << this->grid_spawns.size() << " ";
  for(auto it : this->grid_spawns)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << this->grid_deltas.size() << " ";
  for(auto it : this->grid_deltas)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
}

world_t::world_t()
{
}

world_t::world_t(std::istream &is)
{
  int count;
  ow_assert(is >> count);
  while(count--)
  {
    oid_t where;
    ow_assert(is >> where);
    grids[where] = cloner_t::g_cloner_get()->create_grid(is);
  }
}

world_t::~world_t()
{
  for(auto it : grids)
  {
    delete it.second;
  }
  grids.clear();
}

void world_t::serialise(std::ostream &os)
{
  os << " " << grids.size() << " ";
  for(auto it : grids)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
}

grid_t *world_t::get_grid(oid_t id)
{
  auto it = grids.find(id);
  if(it != grids.end())
  {
    return it->second;
  }
  return NULL;
}

world_delta *world_t::compute_delta(context_t ctx) const
{
  world_delta *wd = new world_delta();
  for(auto it : this->grids)
  {
    ctx.grid_id = it.first;
    wd->grid_deltas[it.first] = it.second->compute_delta(ctx);;
  }
  return wd;
}

void world_t::apply_delta(world_delta *wd)
{
  for(auto it : wd->grid_spawns)
  {
    grids[it.first] = cloner_t::g_cloner_get()->create_grid(it.second);
  }
  for(auto it : wd->grid_deltas)
  {
    grids[it.first]->apply_delta(it.second);
    if(grids[it.first]->get_suicide())
    {
      delete grids[it.first];
      grids.erase(it.first);
    }
  }
}
