#include "slicer.h"

using namespace slicer;

slice_t::slice_t()
{
}

slice_t slice(context_t *ctx)
{
  slice_t next_slice;
  next_slice.gd = ctx->get_grid()->compute_delta();
  return next_slice;
}

void apply(context_t *ctx, slice_t slice)
{
  ctx->get_grid()->apply_delta(slice.gd);
}
