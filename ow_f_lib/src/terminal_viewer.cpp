#include "terminal_viewer.h"
#include "object_viewer.h"

terminal_viewer_t::terminal_viewer_t(world_t *w)
{
  this -> world = w;
  for(int count = 50; count; count--)
  {
    std::cout << "\n";
    y_pos++;
  }
  set_pos(0, 0);
  render_thread = std::thread(std::bind(&terminal_viewer_t::render, this));
}

terminal_viewer_t::~terminal_viewer_t()
{
  kill = true;
  render_thread.join();
}

void terminal_viewer_t::set_cell_renderer(std::function<void(terminal_viewer_t *, context_t, cell_t *)> renderer)
{
  cell_renderer = renderer;
}

void terminal_viewer_t::set_structure_renderer(namer_t id, std::function<void(terminal_viewer_t *, context_t, structure_t *)> renderer)
{
  structure_renderers[id] = renderer;
}

void terminal_viewer_t::set_walker_renderer(namer_t id, std::function<void(terminal_viewer_t *, context_t, walker_t *)> renderer)
{
  walker_renderers[id] = renderer;
}

void terminal_viewer_t::render()
{
  while(!kill)
  {
    world_viewer_t wv(world);
    context_t ctx;
    ctx.world = &wv;
    for(auto it : wv.get_grids())
    {
      ctx.grid_id = it.first;
      render_grid(ctx, it.second);
    }
  }
}

void terminal_viewer_t::render_grid(context_t ctx, grid_t *g)
{
  grid_viewer_t gv(g);
  for(int i = 0; i < gv.get_size().x; i++)
  {
    for(int j = 0; j < gv.get_size().y; j++)
    {
      set_pos(i, j);
      render_cell(ctx, gv.at({i, j}));
    }
  }
  for(auto it : gv.get_structures())
  {
    set_pos(it.second->get_position().x, it.second->get_position().y);
    ctx.element_id = it.first;
    render_structure(ctx, it.second);
  }
  for(auto it : gv.get_walkers())
  {
    set_pos(it.second->get_position().x, it.second->get_position().y);
    ctx.element_id = it.first;
    render_walker(ctx, it.second);
  }
}

void terminal_viewer_t::render_cell(context_t ctx, cell_t *c)
{
  if(cell_renderer)
  {
    cell_renderer(this, ctx, c);
    return;
  }
  std::cout << " ";
  y_pos++;
}

void terminal_viewer_t::render_structure(context_t ctx, structure_t *s)
{
  if(structure_renderers[s->get_clone_identifier()])
  {
    structure_renderers[s->get_clone_identifier()](this, ctx, s);
    return;
  }
  std::cout << "s";
  y_pos++;
}

void terminal_viewer_t::render_walker(context_t ctx, walker_t *w)
{
  if(walker_renderers[w->get_clone_identifier()])
  {
    walker_renderers[w->get_clone_identifier()](this, ctx, w);
    return;
  }
  std::cout << "w";
  y_pos++;
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
