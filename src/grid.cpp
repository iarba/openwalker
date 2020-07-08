#include "grid.h"

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

cell_t::cell_t()
{
}

cell_t::cell_t(std::istream& is)
{
  is >> temporary >> persistent;
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
  value_t rval = get(name, &found, NULL);
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
  is >> size.x >> size.y;
  grid = new cell_t*[size.x];
  for(int i = 0; i < size.x; i++)
  {
    grid[i] = new cell_t[size.y];
  }
  is >> suicide;
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      glm::ivec2 where = {i, j};
      grid[i][j] = cell_t(is);
    }
  }
  int count;
  is >> count;
  while(count--)
  {
    oid_t where;
    is >> where;
    structures[where] = cloner_t::g_cloner_get()->create_structure(is);
  }
  is >> count;
  while(count--)
  {
    oid_t where;
    is >> where;
    walkers[where] = cloner_t::g_cloner_get()->create_walker(is);
  }
  clone_identifier = cloner_registry__grid_cloner;
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

void grid_t::serialise(std::ostream& os)
{
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
}

void grid_t::copy_into(grid_t *other)
{
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

grid_delta *grid_t::compute_delta(context_t ctx) const
{
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
  return gd;
}

void grid_t::apply_delta(grid_delta *gd)
{
  for(int i = 0; i < this->size.x; i++)
  {
    for(int j = 0; j < this->size.y; j++)
    {
      at({i, j})->discard();
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
    structures[it.first] = cloner_t::g_cloner_get()->create_structure(it.second);
  }
  for(auto it : gd->structure_deltas)
  {
    structures[it.first]->apply_delta(it.second);
    if(structures[it.first]->get_suicide())
    {
      delete structures[it.first];
      structures.erase(it.first);
    }
  }
  for(auto it : gd->walker_spawns)
  {
    walkers[it.first] = cloner_t::g_cloner_get()->create_walker(it.second);
  }
  for(auto it : gd->walker_deltas)
  {
    walkers[it.first]->apply_delta(it.second);
    if(walkers[it.first]->get_suicide())
    {
      delete walkers[it.first];
      walkers.erase(it.first);
    }
  }
  this->suicide = gd->suicide;
}

bool grid_t::get_suicide()
{
  return suicide;
}

namer_t grid_t::get_clone_identifier()
{
  return clone_identifier;
}
