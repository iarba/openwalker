#include "cell.h"

//first, define the cloner class

structure_t *cell::cell_constructor::instantiate(structure_t *s)
{
  // we are guaranteed s to be a cell, so we can go ahead and cast it
  cell *c = (cell *)s;
  // we cannot return what is given to us, we have to allocate a new instance, as per function name.
  // luckily, we only need position, which is publicly available in the base class
  return new cell(c->get_position());
}

structure_t *cell::cell_constructor::deserialise(std::istream &is)
{
  // we're forced to delegate construction within the class
  return new cell(is);
}

cell::cell(glm::ivec2 position) : structure_t(position)
{
  // yep, it's that easy
  // time to set the clone identifier
  clone_identifier = cell__cell;
  // that variable is defined by the 2 functions in the header
  // if you're curious, it's pretty much a concattenation of the zone allocator and the zone index
}

cell::cell(std::istream &is) : structure_t(is)
{
  clone_identifier = cell__cell;
}

void cell::append_influence_delta(influence_delta &id, context_t ctx) const
{
  // ok this is a toughie, because we can look, but we shouldn't really touch.
  // you can access the parent grid using the provided context.
  grid_t *g = ctx.world->get_grid(ctx.grid_id);
  // it shouldn't be that hard to understand what's going on
  // context has a pointer to a world, a grid id and an element id.

  // we are passed an influence delta - basically the footprint of previous cells queried.
  // looking at it, it seems like a scary data structure, right?
  id.cell_temporary_setters[this->get_position()];
  // well that's how we access the properties at the current location
  // and here's how we access the value of a specific property at said location
  id.cell_temporary_setters[this->get_position()][cell__neighbors];
  // and now to alter it;
  // id.cell_temporary_setters[this->get_position()][cell__neighbors]++;
  // ain't that easy?

  // but wait, what about the other 8 neighbors?
  // let's iterate
  for(int dx = -1; dx <= 1; dx++)
  {
    for(int dy = -1; dy <= 1; dy++)
    {
      if((dx == 0) && (dy == 0))
      {
        // we don't count ourselves as neighbors
        continue;
      }
      glm::ivec2 p = this->position + glm::ivec2(dx, dy);
      // wait, we want to wrap wround if needed. so (50, y) should point to (0, y) and (x, -1) to (x, 49)
      if(p.x < 0)
      {
        // let's not hardcode
        p.x += g->get_size().x;
      }
      if(p.x > g->get_size().x)
      {
        p.x -= g->get_size().x;
      }
      if(p.y < 0)
      {
        p.y += g->get_size().y;
      }
      if(p.y > g->get_size().y)
      {
        p.y -= g->get_size().y;
      }
      // that's it, we wrapped. now to commit changes
      id.cell_temporary_setters[p][cell__neighbors]++;
    }
  }
}

// now for the loader function
void cell_load()
{
  imp_zone(cell);
  imp(cell, cell);
  imp(cell, neighbors);
  // next call is gonna get a bit spooky because it's a singleton
  cloner_t::g_cloner_get()->reg_structure(cell__cell, new cell::cell_constructor());
  // we basically bound the cell constructor to the clone identifier, and everything else will be handled under the roof
}

// that's it, the structure is defined. now back to main to call the loader.
