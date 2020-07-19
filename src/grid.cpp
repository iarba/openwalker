#include "grid.h"
#include "misc_utils.h"

abstract_grid_constructor_base::~abstract_grid_constructor_base()
{
}

grid_delta::grid_delta()
{
}

grid_delta::grid_delta(const grid_delta *other)
{
  grid_delta gd;
  for(auto it : other->structure_spawns)
  {
    this->structure_spawns[it.first] = cloner_t::g_cloner_get()->create_structure(it.second);
  }
  for(auto it : other->structure_deltas)
  {
    this->structure_deltas[it.first] = new structure_delta(it.second);
  }
  for(auto it : other->walker_spawns)
  {
    this->walker_spawns[it.first] = cloner_t::g_cloner_get()->create_walker(it.second);
  }
  for(auto it : other->walker_deltas)
  {
    this->walker_deltas[it.first] = new walker_delta(it.second);
  }
  this->inf_delta = other->inf_delta;
  this->suicide = other->suicide;
}

grid_delta::grid_delta(std::istream &is)
{
  ow_assert(is >> suicide);
  inf_delta = influence_delta(is);
  int count;
  oid_t where;
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where);
    structure_spawns[where] = cloner_t::g_cloner_get()->create_structure(is);
  }
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where);
    structure_deltas[where] = new structure_delta(is);
  }
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where);
    walker_spawns[where] = cloner_t::g_cloner_get()->create_walker(is);
  }
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where);
    walker_deltas[where] = new walker_delta(is);
  }
}

grid_delta::~grid_delta()
{
  for(auto it : this->structure_spawns)
  {
    delete it.second;
  }
  for(auto it : this->structure_deltas)
  {
    delete it.second;
  }
  for(auto it : this->walker_spawns)
  {
    delete it.second;
  }
  for(auto it : this->walker_deltas)
  {
    delete it.second;
  }
}

void grid_delta::serialise(std::ostream &os)
{
  os << " " << suicide << " ";
  inf_delta.serialise(os);
  os << " " << structure_spawns.size() << " ";
  for(auto it : this->structure_spawns)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << structure_deltas.size() << " ";
  for(auto it : this->structure_deltas)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << walker_spawns.size() << " ";
  for(auto it : this->walker_spawns)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
  os << " " << walker_deltas.size() << " ";
  for(auto it : this->walker_deltas)
  {
    os << " " << it.first << " ";
    it.second->serialise(os);
  }
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

grid_delta *grid_t::compute_delta(context_t ctx) const
{
  critical_lock.lock();
  grid_delta *gd = new grid_delta();
  for(auto it : this->structures)
  {
    ctx.element_id = it.first;
    gd->structure_deltas[it.first] = it.second->compute_delta(ctx);
  }
  for(auto it : this->walkers)
  {
    ctx.element_id = it.first;
    gd->walker_deltas[it.first] = it.second->compute_delta(ctx);
  }
  for(auto it : this->structures)
  {
    ctx.element_id = it.first;
    it.second->append_influence_delta(gd->inf_delta, ctx);
  }
  for(auto it : this->walkers)
  {
    ctx.element_id = it.first;
    it.second->append_influence_delta(gd->inf_delta, ctx);
  }
  critical_lock.unlock();
  return gd;
}

void grid_t::apply_delta(grid_delta *gd, context_t ctx)
{
  critical_lock.lock();
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      at({i, j})->discard();
    }
  }
  for(auto it : gd->inf_delta.cell_unsetters)
  {
    cell_t *c = at(it.first);
    for(auto it2 : it.second)
    {
      c->unset(it2.first);
    }
  }
  for(auto it : gd->inf_delta.cell_persistent_setters)
  {
    cell_t *c = at(it.first);
    for(auto it2 : it.second)
    {
      c->set_persistent(it2.first, it2.second);
    }
  }
  for(auto it : gd->inf_delta.cell_temporary_setters)
  {
    cell_t *c = at(it.first);
    for(auto it2 : it.second)
    {
      c->set(it2.first, it2.second);
    }
  }
  for(auto it : gd->structure_spawns)
  {
    ctx.element_id = it.first;
    structures[it.first] = cloner_t::g_cloner_get()->create_structure(it.second);
    structures[it.first]->trigger_create(ctx);
  }
  for(auto it : gd->structure_deltas)
  {
    structures[it.first]->apply_delta(it.second);
    if(structures[it.first]->get_suicide())
    {
      ctx.element_id = it.first;
      structures[it.first]->trigger_delete(ctx);
      delete structures[it.first];
      structures.erase(it.first);
    }
  }
  for(auto it : gd->walker_spawns)
  {
    ctx.element_id = it.first;
    walkers[it.first] = cloner_t::g_cloner_get()->create_walker(it.second);
    walkers[it.first]->trigger_create(ctx);
  }
  for(auto it : gd->walker_deltas)
  {
    walkers[it.first]->apply_delta(it.second);
    if(walkers[it.first]->get_suicide())
    {
      ctx.element_id = it.first;
      walkers[it.first]->trigger_delete(ctx);
      delete walkers[it.first];
      walkers.erase(it.first);
    }
  }
  this->suicide = gd->suicide;
  critical_lock.unlock();
}

bool grid_t::get_suicide() const
{
  return suicide;
}

namer_t grid_t::get_clone_identifier() const
{
  return clone_identifier;
}

void grid_t::append_triggers(std::vector<std::pair<event_t, context_t>> &triggers, context_t ctx, std::function<double()> roll)
{
  critical_lock.lock();
  for(auto ev : ieh.on_random)
  {
    if(ev.chance(ctx) > roll())
    triggers.push_back({ev, ctx});
  }
  for(auto it : structures)
  {
    ctx.element_id = it.first;
    it.second->append_triggers(triggers, ctx, roll);
  }
  for(auto it : walkers)
  {
    ctx.element_id = it.first;
    it.second->append_triggers(triggers, ctx, roll);
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
