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
}

glm::ivec2 grid_t::get_size()
{
  return this->size;
}

cell_t *grid_t::at(glm::ivec2 position)
{
  return this->grid[position.x] + position.y;
}
