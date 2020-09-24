#include "world.h"
#include "clone.h"
#include "misc_utils.h"
#include "xoshirowrapper.h"

world_delta::world_delta()
{
}

world_delta::world_delta(const world_delta *other)
{
  this->triggers = other->triggers;
}

world_delta::world_delta(std::istream &is)
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

world_delta::~world_delta()
{
  this->triggers.clear();
}

void world_delta::serialise(std::ostream &os)
{
  os << " " << this->triggers.size() << " ";
  for(auto it : this->triggers)
  {
    it.first.serialise(os);
    os << " " << it.second << " ";
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
  xoshirorandomiser r(ctx.seed);
  for(auto it : this->grids)
  {
    ctx.seed = r.next();
    ctx.grid_id = it.first;
    it.second->append_triggers(wd->triggers, ctx);
  }
  critical_lock.unlock();
  return wd;
}

void world_t::apply_delta(world_delta *wd)
{
  critical_lock.lock();
  for(auto it : wd->triggers)
  {
      context_t ctx = it.second;
      ctx.world = this; // masquerade
      ow_safe_cont(it.first.trigger(ctx));
  }
  critical_lock.unlock();
}

std::map<oid_t, grid_t *> world_t::get_grids()
{
  return grids;
}

void world_t::deletion_queue_maintenance()
{
  critical_lock.lock();
  for(auto w : walker_deletion_queue)
  {
    delete w;
  }
  walker_deletion_queue.clear();
  for(auto s : structure_deletion_queue)
  {
    delete s;
  }
  structure_deletion_queue.clear();
  for(auto g : grid_deletion_queue)
  {
    delete g;
  }
  grid_deletion_queue.clear();
  critical_lock.unlock();  
}
