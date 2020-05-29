#ifndef SLICER_H
#define SLICER_H

#include "_fwd.h"
#include "context.h"
#include "grid.h"

namespace slicer
{
  class slice_t
  {
  public:
    grid_delta gd;
    slice_t();
  };

  slice_t slice(context_t *ctx);
  void apply(context_t *ctx, slice_t slice);
};

#endif // SLICER_H
