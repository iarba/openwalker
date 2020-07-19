#ifndef GOL_GRID_H
#define GOL_GRID_H

#include <openwalker/openwalker.h>
#include "cell.h"

// we have a grid of x*y cells, we need a way to uniquely address each cell
// in openwalker, structures are identified within their parent by oid_t
// which is, conveniently, a pair of 2 ints
// as such, we're gonna consider that id(x, y) = {x. y}
oid_t oid_of(glm::ivec2 position);

// we have cell defined in cell.h, so we're gonna reuse it
def(cell, grid);

// most of this should be the same as for cell
class cranking_grid : public grid_t
{
public:
  class cranking_grid_constructor : public abstract_grid_constructor_base
  {
  public:
    virtual grid_t *instantiate(grid_t *s);
    virtual grid_t *deserialise(std::istream &is);
  };

  cranking_grid(glm::ivec2 size);
  cranking_grid(std::istream &is);
  grid_delta *compute_delta(context_t ctx) const;
};

// don't forget loader function
void grid_load();

#endif // GOL_GRID_H
