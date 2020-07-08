#include "world.h"
#include "clone.h"

world_delta world_delta::instantiate()
{
  world_delta wd;
  for(auto it : this->grid_spawns)
  {
    wd.grid_spawns[it.first] = cloner_t::g_cloner_get()->create_grid(it.second);
  }
  for(auto it : this->grid_deltas)
  {
    wd.grid_deltas[it.first] = it.second.instantiate();
  }
  return wd;
}

world_t::world_t()
{
}

world_t::~world_t()
{
  for(auto it : grids)
  {
    delete it.second;
  }
  grids.clear();
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

world_delta world_t::compute_delta(context_t ctx) const
{
  world_delta wd;
  for(auto it : this->grids)
  {
    ctx.grid_id = it.first;
    wd.grid_deltas[it.first] = it.second->compute_delta(ctx);;
  }
  return wd;
}

void world_t::apply_delta(world_delta wd)
{
  for(auto it : wd.grid_spawns)
  {
    grids[it.first] = it.second;
  }
  std::map<oid_t, grid_t *> new_grids = grids;
  for(auto it : wd.grid_deltas)
  {
    grids[it.first]->apply_delta(it.second);
    if(grids[it.first]->get_suicide())
    {
      delete grids[it.first];
      new_grids.erase(it.first);
    }
  }
  grids = new_grids;
}
