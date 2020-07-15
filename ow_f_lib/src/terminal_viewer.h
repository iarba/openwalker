#ifndef TERMINAL_VIEWER_H
#define TERMINAL_VIEWER_H

#include <openwalker/openwalker.h>
#include <functional>
#include <map>
#include <thread>

class terminal_viewer_t
{
public:
  terminal_viewer_t(world_t *w);
  ~terminal_viewer_t();
  void set_cell_renderer(std::function<void(terminal_viewer_t *, context_t, cell_t *)> renderer);
  void set_structure_renderer(namer_t id, std::function<void(terminal_viewer_t *, context_t, structure_t *)> renderer);
  void set_walker_renderer(namer_t id, std::function<void(terminal_viewer_t *, context_t, walker_t *)> renderer);
  void set_pos(int x, int y);
  int x_pos = 0, y_pos = 0;
protected:
  std::function<void(terminal_viewer_t *, context_t, cell_t *)> cell_renderer;
  std::map<namer_t, std::function<void(terminal_viewer_t *, context_t, structure_t *)>> structure_renderers;
  std::map<namer_t, std::function<void(terminal_viewer_t *, context_t, walker_t *)>> walker_renderers;
  world_t *world;
  std::mutex viewer_lock;
  bool kill = false;
private:
  std::thread render_thread;
  void render();
  void render_grid(context_t ctx, grid_t *g);
  void render_cell(context_t ctx, cell_t *c);
  void render_structure(context_t ctx, structure_t *s);
  void render_walker(context_t ctx, walker_t *w);
};

#endif // TERMINAL_VIEWER_H
