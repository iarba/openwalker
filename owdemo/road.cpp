#include "road.h"
#include "common_prop.h"

#define ROAD_SPRITE_SCALE 8.0f

void road_t::init(sfml_viewer_t *view)
{
  // initialise namers
  imp(owdemo, id_road);
  imp(owdemo, event_road_construct);
  imp(owdemo, event_road_destruct);
  imp(owdemo, tex_road);
  imp_vec(owdemo, spr_road_vec, 16);
  // register events
  event_register(owdemo__event_road_construct, ow_event_helpers::ret_0, road_t::on_construct);
  event_register(owdemo__event_road_destruct, ow_event_helpers::ret_0, road_t::on_destruct);
  // install cloner
  cloner_t::g_cloner_get()->reg_structure(owdemo__id_road, new road_t::road_constructor());
  if(view)
  {
    // load tilemap
    view->load_tex(owdemo__tex_road, "assets/owdemo/road.png");
    // derive sprites from tilemap
    for(int px = 0; px < 4; px++)
    {
      for(int py = 0; py < 4; py++)
      {
        view->define_sprite(owdemo__spr_road_vec[px * 4 + py], owdemo__tex_road, px * ROAD_SPRITE_SCALE, py * ROAD_SPRITE_SCALE, ROAD_SPRITE_SCALE, ROAD_SPRITE_SCALE);
      }
    }
    // configure renderer
    view->set_structure_renderer(owdemo__id_road, road_t::sfml_renderer);
  }
}

structure_t *road_t::road_constructor::instantiate(structure_t *s)
{
  road_t *r = (road_t *)s;
  road_t *nr = new road_t(r->position);
  r->copy_into(nr);
  return nr;
}

structure_t *road_t::road_constructor::deserialise(std::istream &is)
{
  road_t *nr = new road_t(is);
  return nr;
}

void road_t::on_construct(context_t ctx)
{
  grid_t *g = ctx.grid();
  structure_t *s = ctx.structure();
  g->at(s->get_position())->set_persistent(owdemo__property_road_walkable, 1);
}

void road_t::on_destruct(context_t ctx)
{
  grid_t *g = ctx.grid();
  structure_t *s = ctx.structure();
  g->at(s->get_position())->unset(owdemo__property_road_walkable);
}

void road_t::sfml_renderer(sfml_viewer_t::viewer_context_t ctx, structure_t *s)
{
  ctx.ctx.cell_pos = s->get_position();
  int offset = 0;
  // north
  ctx.ctx.cell_pos.x -= 1;
  if(ctx.ctx.valid() && ctx.ctx.cell()->get(owdemo__property_road_walkable, 0, NULL))
  {
    offset += 1;
  }
  // south
  ctx.ctx.cell_pos.x += 2;
  if(ctx.ctx.valid() && ctx.ctx.cell()->get(owdemo__property_road_walkable, 0, NULL))
  {
    offset += 2;
  }
  // north
  ctx.ctx.cell_pos.x -= 1;
  ctx.ctx.cell_pos.y -= 1;
  if(ctx.ctx.valid() && ctx.ctx.cell()->get(owdemo__property_road_walkable, 0, NULL))
  {
    offset += 4;
  }
  // north
  ctx.ctx.cell_pos.y += 2;
  if(ctx.ctx.valid() && ctx.ctx.cell()->get(owdemo__property_road_walkable, 0, NULL))
  {
    offset += 8;
  }
  sf::Sprite *spr = ctx.that->get_sprite(owdemo__spr_road_vec[offset]);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / ROAD_SPRITE_SCALE, SFML_VIEWER_DEF_CEL_SIZE / ROAD_SPRITE_SCALE));
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * s->get_position().x, SFML_VIEWER_DEF_CEL_SIZE * s->get_position().y);
  ctx.that->draw(spr);
}

road_t::road_t(glm::ivec2 position) : structure_t(position)
{ 
  ieh.on_create.push_back(event_t(owdemo__event_construct));
  ieh.on_delete.push_back(event_t(owdemo__event_destruct));
  ieh.on_create.push_back(event_t(owdemo__event_road_construct));
  ieh.on_delete.push_back(event_t(owdemo__event_road_destruct));
  clone_identifier = owdemo__id_road;;
}

road_t::road_t(std::istream &is) : structure_t(is)
{
  clone_identifier = owdemo__id_road;;
}

road_t::~road_t()
{
}
