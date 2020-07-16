#ifndef TERMINAL_VIEWER_H
#define TERMINAL_VIEWER_H

#include <openwalker/openwalker.h>
#include <functional>
#include <map>
#include <thread>

#define DEF_HEIGHT 24
#define DEF_WIDTH 80

class terminal_viewer_t
{
public:
  class viewer_context_t
  {
  public:
    char &at(int x_pos, int y_pos);
    terminal_viewer_t *that;
    context_t ctx;
    int cell_scale;
    int x_pos;
    int y_pos;
  };

  terminal_viewer_t(world_t *w, int height = DEF_HEIGHT, int width = DEF_WIDTH, int cell_scale = 1);
  ~terminal_viewer_t();
  terminal_viewer_t *set_refreshing(bool value);
  terminal_viewer_t *set_auto(bool value);
  void set_cell_renderer(std::function<void(viewer_context_t, cell_t *)> renderer);
  void set_structure_renderer(namer_t id, std::function<void(viewer_context_t, structure_t *)> renderer);
  void set_walker_renderer(namer_t id, std::function<void(viewer_context_t, walker_t *)> renderer);
  bool within_viewport(int x_pos, int y_pos);
  terminal_viewer_t *set(oid_t grid_id);
  terminal_viewer_t *set(glm::ivec2 viewport_pos);
  terminal_viewer_t *set(glm::ivec2 viewport_pos, oid_t grid_id);
  terminal_viewer_t *render_now();
protected:
  char &at(int x_pos, int y_pos);
  void flush();
  terminal_viewer_t *force_release_lock();
  bool refreshing = false;
  std::function<void(viewer_context_t, cell_t *)> cell_renderer;
  std::map<namer_t, std::function<void(viewer_context_t, structure_t *)>> structure_renderers;
  std::map<namer_t, std::function<void(viewer_context_t, walker_t *)>> walker_renderers;
  world_t *world;
  std::mutex viewer_lock;
  bool kill = false;
  int x_pos = 0, y_pos = 0;
  int height, width;
  int cell_scale;
  char **buf;
  oid_t grid_id;
  glm::ivec2 viewport_pos;
  void set_pos(int x, int y);
  int gen = 0;
  bool automatic = false;
  std::mutex render_lock;
private:
  std::thread render_thread;
  void render();
  void render_grid(viewer_context_t ctx, grid_t *g);
  void render_cell(viewer_context_t ctx, cell_t *c);
  void render_structure(viewer_context_t ctx, structure_t *s);
  void render_walker(viewer_context_t ctx, walker_t *w);
};

#undef DEF_HEIGHT
#undef DEF_WIDTH

#endif // TERMINAL_VIEWER_H
