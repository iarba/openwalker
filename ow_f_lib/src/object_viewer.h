#ifndef OBJECT_VIEWER_H
#define OBJECT_VIEWER_H

#include <openwalker/openwalker.h>

class world_viewer_t : public world_t
{
public:
  world_viewer_t(world_t *base);
  virtual ~world_viewer_t();
};

class grid_viewer_t : public grid_t
{
public:
  grid_viewer_t(grid_t *base);
  virtual ~grid_viewer_t();
};

#endif // OBJECT_VIEWER_H
