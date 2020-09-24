#include "grid.h"
#include "misc_utils.h"
#include "xoshirowrapper.h"

abstract_grid_constructor_base::~abstract_grid_constructor_base()
{
}

cell_t::cell_t()
{
}

cell_t::cell_t(std::istream& is)
{
  ow_assert(is >> temporary >> persistent);
}

void cell_t::serialise(std::ostream& os)
{
  os << " " << temporary << " " << persistent << " ";
}

void cell_t::copy_into(cell_t *other)
{
  other->persistent = this->persistent;
  other->temporary = this->temporary;
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

value_t cell_t::get(namer_t name, value_t def, bool *found, properties_t *delta_persistent_setter, properties_t *delta_temporary_setter)
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
  if(delta_temporary_setter)
  {
    it = delta_temporary_setter->find(name);
    if(it != delta_temporary_setter->end())
    {
      *found = true;
      rval = it -> second;
    }
  }
  it = persistent.find(name);
  if(it != persistent.end())
  {
    *found = true;
    rval = it -> second;
  }
  if(delta_persistent_setter)
  {
    it = delta_persistent_setter->find(name);
    if(it != delta_persistent_setter->end())
    {
      *found = true;
      rval = it -> second;
    }
  }
  return rval;
}

value_t cell_t::get(namer_t name, value_t def, bool *found)
{
  return get(name, def, found, NULL, NULL);
}

value_t cell_t::get(namer_t name)
{
  bool found;
  value_t rval = get(name, 0, &found);
  if(found)
  {
    return rval;
  }
  throw std::logic_error("property not found");
}

def(cloner_registry, grid_cloner);

void grid_t::load()
{
  imp(cloner_registry, grid_cloner);
  cloner_t::g_cloner_get()->reg_grid(cloner_registry__grid_cloner, new grid_constructor());
}

grid_t *grid_t::grid_constructor::instantiate(grid_t *g)
{
  grid_t *ng = new grid_t(g->size);
  g->copy_into(ng);
  return ng;
}

grid_t *grid_t::grid_constructor::deserialise(std::istream &is)
{
  grid_t *ng = new grid_t(is);
  return ng;
}

grid_t::grid_t(glm::ivec2 size)
{
  this->size = size;
  grid = new cell_t*[size.x];
  for(int i = 0; i < size.x; i++)
  {
    grid[i] = new cell_t[size.y];
  }
  ieh.on_random.push_back(event_t(ow_d_events__grid_suicide));
  ieh.on_random.push_back(event_t(ow_d_events__aging));
  clone_identifier = cloner_registry__grid_cloner;
}

grid_t::grid_t(std::istream& is)
{
  ow_assert(is >> size.x >> size.y);
  grid = new cell_t*[size.x];
  for(int i = 0; i < size.x; i++)
  {
    grid[i] = new cell_t[size.y];
  }
  ow_assert(is >> suicide);
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      grid[i][j] = cell_t(is);
    }
  }
  int count;
  ow_assert(is >> count);
  while(count--)
  {
    oid_t where;
    ow_assert(is >> where);
    structures[where] = cloner_t::g_cloner_get()->create_structure(is);
  }
  ow_assert(is >> count);
  while(count--)
  {
    oid_t where;
    ow_assert(is >> where);
    walkers[where] = cloner_t::g_cloner_get()->create_walker(is);
  }
  ow_assert(is >> ieh);
  clone_identifier = cloner_registry__grid_cloner;
}

grid_t::~grid_t()
{
  critical_lock.lock();
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
  critical_lock.unlock();
}

void grid_t::serialise(std::ostream& os) const
{
  critical_lock.lock();
  os << " " << get_clone_identifier() << " "; // only base class is required to do this.
  os << " " << size.x << " " << size.y << " ";
  os << " " << suicide << " ";
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      glm::ivec2 where = {i, j};
      at(where)->serialise(os);
    }
  }
  os << " " << structures.size() << " ";
  for(auto it : this->structures)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << walkers.size() << " ";
  for(auto it : this->walkers)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << ieh << " ";
  critical_lock.unlock();
}

void grid_t::copy_into(grid_t *other) const
{
  critical_lock.lock();
  // assuming size and cell memory allocation is handled by constructor
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      glm::ivec2 where = {i, j};
      this->at(where)->copy_into(other->at(where));
    }
  }
  for(auto it : this->structures)
  {
    other->structures[it.first] = cloner_t::g_cloner_get()->create_structure(it.second);
  }
  for(auto it : this->walkers)
  {
    other->walkers[it.first] = cloner_t::g_cloner_get()->create_walker(it.second);
  }
  other->suicide = this->suicide;
  other->ieh = this->ieh;
  critical_lock.unlock();
}

glm::ivec2 grid_t::get_size() const
{
  return this->size;
}

cell_t *grid_t::at(glm::ivec2 position) const
{
  return this->grid[position.x] + position.y;
}

structure_t *grid_t::get_structure(oid_t id) const
{
  auto it = structures.find(id);
  if(it != structures.end())
  {
    return it->second;
  }
  return NULL;
}

walker_t *grid_t::get_walker(oid_t id) const
{
  auto it = walkers.find(id);
  if(it != walkers.end())
  {
    return it->second;
  }
  return NULL;
}

bool grid_t::get_suicide() const
{
  return suicide;
}

namer_t grid_t::get_clone_identifier() const
{
  return clone_identifier;
}

void grid_t::append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx) const
{
  critical_lock.lock();
  xoshirorandomiser r(ctx.seed);
  for(auto ev : ieh.on_random)
  {
    ctx.seed = r.next();
    if(ev.chance(ctx))
    triggers.push_back({ev, ctx});
  }
  for(auto it : structures)
  {
    ctx.seed = r.next();
    ctx.element_id = it.first;
    it.second->append_triggers(triggers, ctx);
  }
  for(auto it : walkers)
  {
    ctx.seed = r.next();
    ctx.element_id = it.first;
    it.second->append_triggers(triggers, ctx);
  }
  critical_lock.unlock();
}

void grid_t::trigger_create(context_t ctx)
{
  critical_lock.lock();
  for(auto &event : ieh.on_create)
  {
    event.trigger(ctx);
  }
  critical_lock.unlock();
}

void grid_t::trigger_delete(context_t ctx)
{
  critical_lock.lock();
  for(auto &event : ieh.on_delete)
  {
    event.trigger(ctx);
  }
  critical_lock.unlock();
}

std::map<oid_t, structure_t *> grid_t::get_structures()
{
  return structures;
}

std::map<oid_t, walker_t *> grid_t::get_walkers()
{
  return walkers;
}
