#include "sfml_viewer.h"
#include "object_viewer.h"
#include <chrono>

void printFPS()
{
  static std::chrono::time_point<std::chrono::system_clock> oldTime = std::chrono::high_resolution_clock::now();
  static int fps;
  fps++;
  if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{1})
  {
    oldTime = std::chrono::high_resolution_clock::now();
    std::cout << "FPS: " << fps <<  "\n";
    fps = 0;
  }
}

bool namers_defined = false;

#define ZOOM_SENS 1.1

sfml_viewer_t::sfml_viewer_t(world_t *w, manipulator_t *man)
{
  this -> w = w;
  this -> man = man;
  if(!namers_defined)
  {
    namers_defined = true;
    imp_zone(sfml_viewer_def_res);
    imp(sfml_viewer_def_res, tex);
    imp(sfml_viewer_def_res, str_spr);
    imp(sfml_viewer_def_res, wal_spr);
    imp(sfml_viewer_def_res, cel_spr);
  }
  load_tex(sfml_viewer_def_res__tex, "assets/def.png");
  define_sprite(sfml_viewer_def_res__str_spr, sfml_viewer_def_res__tex, 256, 0, 256, 256);
  define_sprite(sfml_viewer_def_res__wal_spr, sfml_viewer_def_res__tex, 0, 256, 256, 256);
  define_sprite(sfml_viewer_def_res__cel_spr, sfml_viewer_def_res__tex, 0, 0, 256, 256);
}

sfml_viewer_t::~sfml_viewer_t()
{
  for(auto it : sprites)
  {
    delete it.second;
  }
  for(auto it : textures)
  {
    delete it.second;
  }
  if(window)
  {
    delete window;
  }
}

void sfml_viewer_t::start_render()
{
  window = new sf::RenderWindow(sf::VideoMode(800, 600), "OpenWalker");
  view_width = 800;
  view_height = 600;
  reset_camera();
  update_camera();
  window->setFramerateLimit(0);
  window->setVerticalSyncEnabled(false);
  while(!man->is_closed())
  {
    printFPS();
    sf::Event sfev;
    while(window->pollEvent(sfev))
    {
      if(sfev.type == sf::Event::Closed)
      {
        man->close();
      }
      if(sfev.type == sf::Event::Resized)
      {
        view_width = sfev.size.width;
        view_height = sfev.size.height;
        update_camera();
      }
      if(sfev.type == sf::Event::MouseWheelMoved)
      {
        if(sfev.mouseWheel.delta > 0)
        {
          zoom_lvl /= std::pow(ZOOM_SENS, std::abs(sfev.mouseWheel.delta));
        }
        else
        {
          zoom_lvl *= std::pow(ZOOM_SENS, std::abs(sfev.mouseWheel.delta));
        }
        update_camera();
      }
      if(sfev.type == sf::Event::MouseButtonPressed)
      {
        if(sfev.mouseButton.button == sf::Mouse::Left)
        {
          drag_mode = true;
        }
      }
      if(sfev.type == sf::Event::MouseButtonReleased)
      {
        if(sfev.mouseButton.button == sf::Mouse::Left)
        {
          drag_mode = false;
        }
      }
      if(sfev.type == sf::Event::MouseMoved)
      {
        
        auto mpos = sf::Vector2i(sfev.mouseMove.x, sfev.mouseMove.y);
        if(drag_mode)
        {
          cam_x -= (mpos.x - last_mouse_x) * zoom_lvl;
          cam_y -= (mpos.y - last_mouse_y) * zoom_lvl;
          update_camera();
        }
        last_mouse_x = mpos.x;
        last_mouse_y = mpos.y;
      }
    }
    window->clear(sf::Color::White);
    render();
    window->display();
  }
}

void sfml_viewer_t::load_tex(namer_t t_id, std::string tilemap)
{
  sf::Texture *tex = new sf::Texture();
  tex->loadFromFile(tilemap);
  tex->setSmooth(true);
  textures[t_id] = tex;
}

void sfml_viewer_t::define_sprite(namer_t s_id, namer_t t_id, int tlx, int tly, int sizex, int sizey)
{
  sf::Sprite *spr = new sf::Sprite();
  spr->setTexture(*get_texture(t_id));
  spr->setTextureRect(sf::IntRect(tlx, tly, sizex, sizey));
  sprites[s_id] = spr;
}

void sfml_viewer_t::set_cell_renderer(std::function<void(viewer_context_t, cell_t *)> renderer)
{
  cell_renderer = renderer;
}

void sfml_viewer_t::set_structure_renderer(namer_t id, std::function<void(viewer_context_t, structure_t *)> renderer)
{
  structure_renderers[id] = renderer;
}

void sfml_viewer_t::set_walker_renderer(namer_t id, std::function<void(viewer_context_t, walker_t *)> renderer)
{
  walker_renderers[id] = renderer;
}

sf::Sprite *sfml_viewer_t::get_sprite(namer_t s_id)
{
  return sprites[s_id];
}

sf::Texture *sfml_viewer_t::get_texture(namer_t t_id)
{
  return textures[t_id];
}

sfml_viewer_t *sfml_viewer_t::set(oid_t grid_id)
{
  this->grid_id = grid_id;
  return this;
}

sfml_viewer_t *sfml_viewer_t::update_camera()
{
  sf::View v = window->getView();
  v.setCenter(cam_x, cam_y);
  v.setSize(view_width, view_height);
  v.zoom(zoom_lvl);
  window->setView(v);
  return this;
}

sfml_viewer_t *sfml_viewer_t::reset_camera()
{
  zoom_lvl = 0.25;
  glm::ivec2 size = w->get_grid(grid_id)->get_size();
  cam_x = SFML_VIEWER_DEF_CEL_SIZE * size.x / 2;
  cam_y = SFML_VIEWER_DEF_CEL_SIZE * size.y / 2;
  return this;
}

void sfml_viewer_t::draw(sf::Sprite *spr)
{
  window->draw(*spr);
}

void sfml_viewer_t::render()
{
  world_viewer_t wv(w);
  viewer_context_t ctx;
  ctx.ctx.world = &wv;
  ctx.ctx.grid_id = grid_id;
  ctx.that = this;
  render_grid(ctx, wv.get_grid(grid_id));
}

void sfml_viewer_t::render_grid(viewer_context_t ctx, grid_t *g)
{
  for(int i = 0; i < g->get_size().x; i++)
  {
    for(int j = 0; j < g->get_size().y; j++)
    {
      ctx.ctx.cell_pos = {i, j};
      render_cell(ctx, g->at({i, j}));
    }
  }
  for(auto it : g->get_structures())
  {
    ctx.ctx.element_id = it.first;
    render_structure(ctx, it.second);
  }
  for(auto it : g->get_walkers())
  {
    ctx.ctx.element_id = it.first;
    render_walker(ctx, it.second);
  }
}

void sfml_viewer_t::render_cell(viewer_context_t ctx, cell_t *c)
{
  if(cell_renderer)
  {
    cell_renderer(ctx, c);
    return;
  }
  sf::Sprite *spr = get_sprite(sfml_viewer_def_res__cel_spr);
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * ctx.ctx.cell_pos.x, SFML_VIEWER_DEF_CEL_SIZE * ctx.ctx.cell_pos.y);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / 256.0f, SFML_VIEWER_DEF_CEL_SIZE / 256.0f));
  draw(spr);
}

void sfml_viewer_t::render_structure(viewer_context_t ctx, structure_t *s)
{
  if(structure_renderers[s->get_clone_identifier()])
  {
    structure_renderers[s->get_clone_identifier()](ctx, s);
    return;
  }
  sf::Sprite *spr = get_sprite(sfml_viewer_def_res__str_spr);
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * s->get_position().x, SFML_VIEWER_DEF_CEL_SIZE * s->get_position().y);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / 256.0f, SFML_VIEWER_DEF_CEL_SIZE / 256.0f));
  draw(spr);
}

void sfml_viewer_t::render_walker(viewer_context_t ctx, walker_t *w)
{
  if(walker_renderers[w->get_clone_identifier()])
  {
    walker_renderers[w->get_clone_identifier()](ctx, w);
    return;
  }
  sf::Sprite *spr = get_sprite(sfml_viewer_def_res__wal_spr);
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * w->get_position().x, SFML_VIEWER_DEF_CEL_SIZE * w->get_position().y);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / 256.0f, SFML_VIEWER_DEF_CEL_SIZE / 256.0f));
  draw(spr);
}
