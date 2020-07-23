#include "water_carrier.h"
#include "common_prop.h"

#define WATER_CARRIER_SPRITE_SCALE 64.0f
std::function<bool(context_t)> func;

void water_carrier_t::init(sfml_viewer_t *view)
{
  // initialise namers
  imp(owdemo, id_water_carrier);
  imp(owdemo, nvp_water_carrier_pathfinding);
  imp(owdemo, tex_water_carrier);
  imp(owdemo, spr_water_carrier);
  // install cloner
  cloner_t::g_cloner_get()->reg_walker(owdemo__id_water_carrier, new water_carrier_t::water_carrier_constructor());
  // register pathfinding function
  func = water_carrier_t::pathfinding_function;
  dynamic_named_pointer_manager_t::g_dnpm_get()->set(owdemo__nvp_water_carrier_pathfinding, reinterpret_cast<void*>(&func));
  if(view)
  {
    // load tilemap
    view->load_tex(owdemo__tex_water_carrier, "assets/owdemo/water_carrier.png");
    // derive sprites from tilemap
    view->define_sprite(owdemo__spr_water_carrier, owdemo__tex_water_carrier, 0, 0, WATER_CARRIER_SPRITE_SCALE, WATER_CARRIER_SPRITE_SCALE);
    // configure renderer
    view->set_walker_renderer(owdemo__id_water_carrier, water_carrier_t::sfml_renderer);
  }
}

bool water_carrier_t::pathfinding_function(context_t ctx)
{
  return ctx.cell()->get(owdemo__property_road_walkable, 0, NULL) == 1;
}

walker_t *water_carrier_t::water_carrier_constructor::instantiate(walker_t *w)
{
  water_carrier_t *wc = (water_carrier_t *)w;
  water_carrier_t *nwc = new water_carrier_t(wc->position);
  wc->copy_into(nwc);
  return nwc;
}

walker_t *water_carrier_t::water_carrier_constructor::deserialise(std::istream &is)
{
  water_carrier_t *nr = new water_carrier_t(is);
  return nr;
}

void water_carrier_t::sfml_renderer(sfml_viewer_t::viewer_context_t ctx, walker_t *w)
{
  sf::Sprite *spr = ctx.that->get_sprite(owdemo__spr_water_carrier);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / WATER_CARRIER_SPRITE_SCALE, SFML_VIEWER_DEF_CEL_SIZE / WATER_CARRIER_SPRITE_SCALE));
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * w->get_position().x, SFML_VIEWER_DEF_CEL_SIZE * w->get_position().y);
  ctx.that->draw(spr);
}

water_carrier_t::water_carrier_t(glm::ivec2 position) : random_walker_t(position, owdemo__nvp_water_carrier_pathfinding)
{
  speed = OWDEMO_BASE_WALKER_SPEED * 1;
  clone_identifier = owdemo__id_water_carrier;
}

water_carrier_t::water_carrier_t(std::istream &is) : random_walker_t(is)
{
  clone_identifier = owdemo__id_water_carrier;
}

water_carrier_t::~water_carrier_t()
{
}
