#include "cranking_grid.h"

oid_t oid_of(glm::ivec2 position)
{
  // we do as mentioned in the header
  return {position.x, position.y};
}

grid_t *cranking_grid::cranking_grid_constructor::instantiate(grid_t *s)
{
  grid_t *ng = new cranking_grid(s->get_size());
  // this one is a bit more complicated, because we need to transfer base data
  // luckily, there's a function for that
  s->copy_into(ng);
  return ng;
}
 
grid_t *cranking_grid::cranking_grid_constructor::deserialise(std::istream &is)
{
  return new cranking_grid(is);
}

cranking_grid::cranking_grid(glm::ivec2 size) : grid_t(size)
{
  clone_identifier = cell__grid;
}

cranking_grid::cranking_grid(std::istream &is) : grid_t(is)
{
  clone_identifier = cell__grid;
}

// most of the functions above are really straightforward

grid_delta *cranking_grid::compute_delta(context_t ctx) const
{
  // okay, remember the golden rule, no editing.
  // first, let's do a lot of the dirty work and interogate all children.
  // we do so by calling parent function
  grid_delta *gd = this->grid_t::compute_delta(ctx);
  // we could go one by one and check all cells, including ones nowhere near anything
  // but we can also use information within the delta to optimise this.
  // we're gonna check cells with at least 1 neighbour, as they will have an influence_delta entry
  for(auto influence_entry : gd->inf_delta.cell_temporary_setters)
  {
    glm::ivec2 position = influence_entry.first;
    value_t value = influence_entry.second[cell__neighbors];
    // sneaky, right? we also need to fetch the cell at given location, if it exists
    cell *c = (cell *)get_structure(oid_of(position));
    // we implement the rules of game of life - look them up if you don't know them
    if(c && (value < 2 || value > 3))
    {
      // this cell needs to die, but don't touch it, instead we touch the delta
      gd->structure_deltas[oid_of(position)]->suicide = true;
    }
    if(!c && value == 3)
    {
      // we need to create a cell
      gd->structure_spawns[oid_of(position)] = new cell(position);
    }
  }
  return gd;
}

void grid_load()
{
  imp(cell, grid);
  cloner_t::g_cloner_get()->reg_grid(cell__grid, new cranking_grid::cranking_grid_constructor());
}
