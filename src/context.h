#ifndef context_h
#define context_h

#include "_fwd.h"
#include "grid.h"

class context_t
{
public:
  context_t();
  grid_t *get_grid();
protected:
  void set_grid(grid_t *grid);
private:
  grid_t *grid;
};

#endif
