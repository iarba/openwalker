#include "terminal_viewer.h"
#include "object_viewer.h"

static char _null;

char &terminal_viewer_t::viewer_context_t::at(int x_pos, int y_pos)
{
  if(x_pos < 0)
  {
    return _null;
  }
  if(y_pos < 0)
  {
    return _null;
  }
  if(x_pos > cell_scale)
  {
    return _null;
  }
  if(y_pos > cell_scale)
  {
    return _null;
  }
  if(!that->within_viewport(this->x_pos + x_pos, this->y_pos + y_pos))
  {
    return _null;
  }
  return that->at(this->x_pos + x_pos, this->y_pos + y_pos);
}

terminal_viewer_t::terminal_viewer_t(world_t *w, int height, int width, int cell_scale)
{
  render_lock.lock();
  this->height = height;
  this->width = width;
  this->cell_scale = cell_scale;
  this->world = w;
  this->buf = new char*[height];
  for(int i = 0; i < height; i++)
  {
    this->buf[i] = new char[width + 1];
    memset(this->buf[i], ' ', width);
    this->buf[i][width] = 0;
    std::cout << "\n";
  }
  render_thread = std::thread(std::bind(&terminal_viewer_t::render, this));
}

terminal_viewer_t::~terminal_viewer_t()
{
  kill = true;
  render_now();
  render_thread.join();
  for(int i = 0; i < height; i++)
  {
    delete[] this->buf[i];
  }
  delete[] this->buf;
}

terminal_viewer_t *terminal_viewer_t::set_refreshing(bool value)
{
  this->refreshing = value;
  return this;
}

terminal_viewer_t *terminal_viewer_t::set_auto(bool value)
{
  this->automatic = value;
  if(automatic)
  {
    force_release_lock();
  }
  return this;
}

void terminal_viewer_t::set_cell_renderer(std::function<void(viewer_context_t, cell_t *)> renderer)
{
  cell_renderer = renderer;
}

void terminal_viewer_t::set_structure_renderer(namer_t id, std::function<void(viewer_context_t, structure_t *)> renderer)
{
  structure_renderers[id] = renderer;
}

void terminal_viewer_t::set_walker_renderer(namer_t id, std::function<void(viewer_context_t, walker_t *)> renderer)
{
  walker_renderers[id] = renderer;
}

// there are viewport coordinates!
bool terminal_viewer_t::within_viewport(int x_pos, int y_pos)
{
  if(x_pos < 0)
  {
    return false;
  }
  if(y_pos < 0)
  {
    return false;
  }
  if(x_pos > height)
  {
    return false;
  }
  if(y_pos > width)
  {
    return false;
  }
  return true;
}

terminal_viewer_t *terminal_viewer_t::set(oid_t grid_id)
{
  this->grid_id = grid_id;
  return this;
}

terminal_viewer_t *terminal_viewer_t::set(glm::ivec2 viewport_pos)
{
  this->viewport_pos = viewport_pos;
  return this;
}

terminal_viewer_t *terminal_viewer_t::set(glm::ivec2 viewport_pos, oid_t grid_id)
{
  set(viewport_pos);
  set(grid_id);
  return this;
}

terminal_viewer_t *terminal_viewer_t::render_now()
{
  force_release_lock();
  return this;
}

// these are viewport coordinates!
char &terminal_viewer_t::at(int x_pos, int y_pos)
{
  return buf[x_pos][y_pos];
}

void terminal_viewer_t::flush()
{
  if(refreshing)
  {
    x_pos = height;
    y_pos = 0;
    set_pos(0, 0);
  }
  for(int i = 0; i < height; i++)
  {
    std::cout << buf[i] << "\n";
  }
}

terminal_viewer_t *terminal_viewer_t::force_release_lock()
{
  render_lock.try_lock();
  render_lock.unlock();
  return this;
}

void terminal_viewer_t::render()
{
  while(!kill)
  {
    render_lock.lock();
    if(kill)
    {
      render_lock.unlock();
      break;
    }
    world_viewer_t wv(world);
    viewer_context_t ctx;
    ctx.cell_scale = cell_scale;
    ctx.ctx.world = &wv;
    ctx.ctx.grid_id = grid_id;
    ctx.that = this;
    render_grid(ctx, wv.get_grid(grid_id));
    flush();
    gen++;
    if(automatic)
    {
      render_lock.unlock();
    }
  }
}

void terminal_viewer_t::render_grid(viewer_context_t ctx, grid_t *g)
{
  grid_viewer_t gv(g);
  for(int i = 0; i < gv.get_size().x; i++)
  {
    for(int j = 0; j < gv.get_size().y; j++)
    {
      ctx.x_pos = i * cell_scale - viewport_pos.x;
      ctx.y_pos = j * cell_scale - viewport_pos.x;
      render_cell(ctx, gv.at({i, j}));
    }
  }
  for(auto it : gv.get_structures())
  {
    ctx.ctx.element_id = it.first;
    ctx.x_pos = it.second->get_position().x * cell_scale - viewport_pos.x;
    ctx.y_pos = it.second->get_position().y * cell_scale - viewport_pos.x;
    render_structure(ctx, it.second);
  }
  for(auto it : gv.get_walkers())
  {
    ctx.ctx.element_id = it.first;
    ctx.x_pos = it.second->get_position().x * cell_scale - viewport_pos.x;
    ctx.y_pos = it.second->get_position().y * cell_scale - viewport_pos.x;
    render_walker(ctx, it.second);
  }
}

void terminal_viewer_t::render_cell(viewer_context_t ctx, cell_t *c)
{
  if(cell_renderer)
  {
    cell_renderer(ctx, c);
    return;
  }
  for(int i = 0; i < ctx.cell_scale; i++)
  {
    for(int j = 0; j < ctx.cell_scale; j++)
    {
      ctx.at(i, j) = '.';
    }
  }
}

void terminal_viewer_t::render_structure(viewer_context_t ctx, structure_t *s)
{
  if(structure_renderers[s->get_clone_identifier()])
  {
    structure_renderers[s->get_clone_identifier()](ctx, s);
    return;
  }
  ctx.at(ctx.cell_scale / 2, ctx.cell_scale / 2) = 's';
}

void terminal_viewer_t::render_walker(viewer_context_t ctx, walker_t *w)
{
  if(walker_renderers[w->get_clone_identifier()])
  {
    walker_renderers[w->get_clone_identifier()](ctx, w);
    return;
  }
  ctx.at(ctx.cell_scale / 2, ctx.cell_scale / 2) = 'w';
}

void terminal_viewer_t::set_pos(int x, int y)
{
  while(x_pos > x)
  {
    std::cout << "\033[A";
    x_pos--;
  }
  while(x_pos < x)
  {
    std::cout << "\033[B";
    x_pos++;
  }
  while(y_pos < y)
  {
    std::cout << "\033[C";
    y_pos++;
  }
  while(y_pos > y)
  {
    std::cout << "\033[D";
    y_pos--;
  }
}
