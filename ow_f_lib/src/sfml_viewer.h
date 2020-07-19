#ifndef SFML_VIEWER_H
#define SFML_VIEWER_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <openwalker/openwalker.h>
#include "manipulator.h"

def_zone(sfml_viewer_def_res);
def(sfml_viewer_def_res, tex);
def(sfml_viewer_def_res, str_spr);
def(sfml_viewer_def_res, wal_spr);
def(sfml_viewer_def_res, cel_spr);

#define SFML_VIEWER_DEF_CEL_SIZE 64.0f

class sfml_viewer_t
{
public:
  class viewer_context_t
  {
  public:
    sfml_viewer_t *that;
    context_t ctx;
  };

  sfml_viewer_t(world_t *w, manipulator_t *man);
  virtual ~sfml_viewer_t();
  void start_render();
  void load_tex(namer_t t_id, std::string tilemap);
  void define_sprite(namer_t s_id, namer_t t_id, int tlx, int tly, int sizex, int sizey);
  void set_cell_renderer(std::function<void(viewer_context_t, cell_t *)> renderer);
  void set_structure_renderer(namer_t id, std::function<void(viewer_context_t, structure_t *)> renderer);
  void set_walker_renderer(namer_t id, std::function<void(viewer_context_t, walker_t *)> renderer);
  sf::Sprite *get_sprite(namer_t s_id);
  sf::Texture *get_texture(namer_t t_id);
  sfml_viewer_t *set(oid_t grid_id);
  sfml_viewer_t *set_camera(float x, float y);
  sfml_viewer_t *set_zoom(float z);
  sfml_viewer_t *update_camera();
  sfml_viewer_t *reset_camera();
  void draw(sf::Sprite *spr);
protected:
  sf::RenderWindow *window = NULL;
  world_t *w;
  manipulator_t *man;
  std::map<namer_t, sf::Texture *> textures;
  std::map<namer_t, sf::Sprite *> sprites;
  std::function<void(viewer_context_t, cell_t *)> cell_renderer;
  std::map<namer_t, std::function<void(viewer_context_t, structure_t *)>> structure_renderers;
  std::map<namer_t, std::function<void(viewer_context_t, walker_t *)>> walker_renderers;
  oid_t grid_id;
  float zoom_lvl = 0.25f;
  float view_width;
  float view_height;
  float cam_x;
  float cam_y;
  float last_mouse_x;
  float last_mouse_y;
  bool drag_mode = false;
private:
  void render();
  void render_grid(viewer_context_t ctx, grid_t *g);
  void render_cell(viewer_context_t ctx, cell_t *c);
  void render_structure(viewer_context_t ctx, structure_t *s);
  void render_walker(viewer_context_t ctx, walker_t *w);
};

#endif // SFML_VIEWER_H
