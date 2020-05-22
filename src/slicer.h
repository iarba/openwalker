#ifndef SLICER_H
#define SLICER_H

#include "context.h"

namespace slicer
{
  class slice_t
  {
  public:
    slice_t();
  };

  slice_t slice(context_t *ctx);
  void apply(context_t *ctx, slice_t slice);
};

#endif // SLICER_H
