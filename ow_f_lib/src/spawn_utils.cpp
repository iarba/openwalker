#include "spawn_utils.h"

bool ctx_to_true(context_t _ctx)
{
  return true;
}

std::vector<context_t> get_neighbouring_cells_of_structure(context_t str_ctx, int limit, std::function<bool(context_t)> filter)
{
  std::vector<context_t> ret_val;
  structure_t str = str_ctx.world->get_grid(str_ctx.grid_id)->get_structure(std_ctx.element_id);
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
        ret_val.push_back(str_ctx);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  //bottom
  str_ctx.cell_pos = {br.x + 1, 0};
  if(str_ctx.valid())
  {
    for(int y = pos.y; y < br.y; y++)
    {
      str_ctx.cell_pos.y = y;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx);
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
        ret_val.push_back(str_ctx);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  //right
  str_ctx.cell_pos = {0, br.y + 1};
  if(str_ctx.valid())
  {
    for(int x = pos.x; x < br.x; x++)
    {
      str_ctx.cell_pos.x = x;
      if(filter(str_ctx))
      {
        ret_val.push_back(str_ctx);
        if(ret_val.size() == limit)
        {
          return ret_val;
        }
      }
    }
  }
  return ret_val;
}
