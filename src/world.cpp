#include "world.h"
#include "clone.h"
#include "misc_utils.h"

world_delta::world_delta()
{
}

world_delta::world_delta(const world_delta *other)
{
  this->is_slice_not_triggers = other->is_slice_not_triggers;
  if(is_slice_not_triggers)
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
  else
  {
    this->triggers = other->triggers;
  }
}

world_delta::world_delta(std::istream &is)
{
  ow_assert(is >> is_slice_not_triggers);
  if(is_slice_not_triggers)
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
  else
  {
    int count;
    ow_assert(is >> count);
    while(count--)
    {
      context_t ctx;
      ow_assert(is >> ctx);
      event_t e(is);
      triggers.push_back({e, ctx});
    }
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
  this->triggers.clear();
}

void world_delta::serialise(std::ostream &os)
{
  os << " " << this->is_slice_not_triggers << " ";
  if(is_slice_not_triggers)
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
  else
  {
    os << " " << this->triggers.size() << " ";
    for(auto it : this->triggers)
    {
      it.first.serialise(os);
      os << " " << it.second << " ";
    }
  }
}

world_t::world_t()
{
}

world_t::world_t(std::istream &is)
{
  int count;
  ow_assert(is >> is_slice_not_triggers);
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
  deletion_queue_maintenance();
  critical_lock.lock();
  for(auto it : grids)
  {
    delete it.second;
  }
  grids.clear();
  critical_lock.unlock();
}

void world_t::serialise(std::ostream &os)
{
  critical_lock.lock();
  os << " " << is_slice_not_triggers << " ";
  os << " " << grids.size() << " ";
  for(auto it : grids)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  critical_lock.unlock();
}

void world_t::copy_into(world_t *other) const
{
  critical_lock.lock();
  other->is_slice_not_triggers = this->is_slice_not_triggers;
  for(auto it : grids)
  {
    other->grids[it.first] = cloner_t::g_cloner_get()->create_grid(it.second);
  }
  critical_lock.unlock();
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
  critical_lock.lock();
  world_delta *wd = new world_delta();
  wd->is_slice_not_triggers = this->is_slice_not_triggers;
  if(is_slice_not_triggers)
  {
    for(auto it : this->grids)
    {
      ctx.grid_id = it.first;
      wd->grid_deltas[it.first] = it.second->compute_delta(ctx);
    }
  }
  else
  {
    for(auto it : this->grids)
    {
      ctx.grid_id = it.first;
      it.second->append_triggers(wd->triggers, ctx, [](){return (double)rand()/(double)RAND_MAX;});
    }
  }
  critical_lock.unlock();
  return wd;
}

void world_t::apply_delta(world_delta *wd)
{
  critical_lock.lock();
  if(wd->is_slice_not_triggers)
  {
    context_t ctx;
    ctx.world = this;
    for(auto it : wd->grid_spawns)
    {
      ctx.grid_id = it.first;
      grids[it.first] = cloner_t::g_cloner_get()->create_grid(it.second);
      grids[it.first]->trigger_create(ctx);
    }
    for(auto it : wd->grid_deltas)
    {
      grids[it.first]->apply_delta(it.second, ctx);
      if(grids[it.first]->get_suicide())
      {
        ctx.grid_id = it.first;
        grids[it.first]->trigger_delete(ctx);
        if(deletion_queue_mode)
        {
          grid_deletion_queue.push_back(grids[it.first]);
        }
        else
        {
          delete grids[it.first];
        }
        grids.erase(it.first);
      }
    }
  }
  else
  {
    for(auto it : wd->triggers)
    {
      context_t ctx = it.second;
      ctx.world = this; // masquerade
      it.first.trigger(ctx);
    }
  }
  is_slice_not_triggers = !wd->is_slice_not_triggers;
  critical_lock.unlock();
}

std::map<oid_t, grid_t *> world_t::get_grids()
{
  return grids;
}

void world_t::set_deletion_queue_usage(bool value)
{
  critical_lock.lock();
  deletion_queue_mode = value;
  critical_lock.unlock();
}

void world_t::deletion_queue_maintenance()
{
  critical_lock.lock();
  for(auto g : grid_deletion_queue)
  {
    delete g;
  }
  grid_deletion_queue.clear();
  critical_lock.unlock();
}
