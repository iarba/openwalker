#include "spawn_utils.h"
#include "ow_f_lib.h"

def(ow_f_lib, event_grid_spawn_deleter);
def(ow_f_lib, event_element_spawn_deleter);
def_dyn_zone(spawner);

void grid_spawn_deleter(context_t ctx)
{
  release_dyn(spawner, ctx.grid_id);
}

void element_spawn_deleter(context_t ctx)
{
  release_dyn(spawner, ctx.element_id);
}

void spawn_utils::load()
{
  imp_dyn_zone(spawner);
  imp(ow_f_lib, event_grid_spawn_deleter);
  imp(ow_f_lib, event_element_spawn_deleter);
  event_register(ow_f_lib__event_grid_spawn_deleter, ow_event_helpers::never, grid_spawn_deleter);
  event_register(ow_f_lib__event_element_spawn_deleter, ow_event_helpers::never, element_spawn_deleter);
}

bool spawn_utils::ctx_to_true(context_t _ctx)
{
  return true;
}

namer_t spawn_utils::attach_to(world_t *w, grid_t *g)
{
  namer_t id;
  acquire_dyn(spawner, id);
  g->ieh.on_delete.push_back(ow_f_lib__event_grid_spawn_deleter);
  w->grids[id] = g;
  return id;
}

namer_t spawn_utils::attach_to(grid_t *g, walker_t *w)
{
  namer_t id;
  acquire_dyn(spawner, id);
  w->ieh.on_delete.push_back(ow_f_lib__event_element_spawn_deleter);
  g->walkers[id] = w;
  return id;
}

namer_t spawn_utils::attach_to(grid_t *g, structure_t *s)
{
  namer_t id;
  acquire_dyn(spawner, id);
  s->ieh.on_delete.push_back(ow_f_lib__event_element_spawn_deleter);
  g->structures[id] = s;
  return id;
}

std::vector<glm::ivec2> spawn_utils::get_neighbouring_cells_of_structure(context_t str_ctx, unsigned int limit, std::function<bool(context_t)> filter)
{
  std::vector<glm::ivec2> ret_val;
  structure_t *str = str_ctx.world->get_grid(str_ctx.grid_id)->get_structure(str_ctx.element_id);
  glm::ivec2 pos = str->get_position();
  glm::ivec2 sz = str->get_size();
  glm::ivec2 br = pos + sz;
  //top
  str_ctx.cell_pos = {pos.x - 1, 0};
  if(str_ctx.valid())
  {
    for(int y = pos.y; y < br.y; y++)
    {
      str_ctx.cell_pos.y = y;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx.cell_pos);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  //bottom
  str_ctx.cell_pos = {br.x, 0};
  if(str_ctx.valid())
  {
    for(int y = pos.y; y < br.y; y++)
    {
      str_ctx.cell_pos.y = y;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx.cell_pos);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  //left
  str_ctx.cell_pos = {0, pos.y - 1};
  if(str_ctx.valid())
  {
    for(int x = pos.x; x < br.x; x++)
    {
      str_ctx.cell_pos.x = x;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx.cell_pos);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  //right
  str_ctx.cell_pos = {0, br.y};
  if(str_ctx.valid())
  {
    for(int x = pos.x; x < br.x; x++)
    {
      str_ctx.cell_pos.x = x;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx.cell_pos);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  return ret_val;
}
