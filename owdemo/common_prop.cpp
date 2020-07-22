#include "common_prop.h"

void common_prop::init(sfml_viewer_t *view)
{
  // initialise namers
  imp(owdemo, property_road_walkable);
  imp(owdemo, property_house_water);
  imp(owdemo, property_cell_occupied);
  imp(owdemo, event_construct);
  imp(owdemo, event_destruct);
  // register events
  event_register(owdemo__event_construct, ow_event_helpers::ret_0, common_prop::on_construct);
  event_register(owdemo__event_destruct, ow_event_helpers::ret_0, common_prop::on_destruct);
}

void common_prop::on_construct(context_t ctx)
{
  grid_t *g = ctx.grid();
  structure_t *s = ctx.structure();
  glm::ivec2 sz = s->get_size();
  for(int dx = 0; dx < sz.x; dx++)
  {
    for(int dy = 0; dy < sz.y; dy++)
    {
      g->at(s->get_position() + glm::ivec2(dx, dy))->set_persistent(owdemo__property_cell_occupied, 1);
    }
  }
}

void common_prop::on_destruct(context_t ctx)
{
  grid_t *g = ctx.grid();
  structure_t *s = ctx.structure();
  glm::ivec2 sz = s->get_size();
  for(int dx = 0; dx < sz.x; dx++)
  {
    for(int dy = 0; dy < sz.y; dy++)
    {
      g->at(s->get_position() + glm::ivec2(dx, dy))->unset(owdemo__property_cell_occupied);
    }
  }
}

