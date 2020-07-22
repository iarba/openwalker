#include "well.h"
#include "common_prop.h"

#define WELL_SPRITE_SCALE 64.0f

void well_t::init(sfml_viewer_t *view)
{
  // initialise namers
  imp(owdemo, id_well);
  imp(owdemo, tex_well);
  imp(owdemo, spr_well);
  // install cloner
  cloner_t::g_cloner_get()->reg_structure(owdemo__id_well, new well_t::well_constructor());
  if(view)
  {
    // load tilemap
    view->load_tex(owdemo__tex_well, "assets/owdemo/well.png");
    // derive sprites from tilemap
    view->define_sprite(owdemo__spr_well, owdemo__tex_well, 0, 0, WELL_SPRITE_SCALE, WELL_SPRITE_SCALE);
    // configure renderer
    view->set_structure_renderer(owdemo__id_well, well_t::sfml_renderer);
  }
}

structure_t *well_t::well_constructor::instantiate(structure_t *s)
{
  well_t *w = (well_t *)s;
  well_t *nw = new well_t(w->position);
  w->copy_into(nw);
  return nw;
}

structure_t *well_t::well_constructor::deserialise(std::istream &is)
{
  well_t *nr = new well_t(is);
  return nr;
}

void well_t::sfml_renderer(sfml_viewer_t::viewer_context_t ctx, structure_t *s)
{
  sf::Sprite *spr = ctx.that->get_sprite(owdemo__spr_well);
  spr->setScale(sf::Vector2f(SFML_VIEWER_DEF_CEL_SIZE / WELL_SPRITE_SCALE * 2, SFML_VIEWER_DEF_CEL_SIZE / WELL_SPRITE_SCALE * 2));
  spr->setPosition(SFML_VIEWER_DEF_CEL_SIZE * s->get_position().x, SFML_VIEWER_DEF_CEL_SIZE * s->get_position().y);
  ctx.that->draw(spr);
}

well_t::well_t(glm::ivec2 position) : structure_t(position, {2, 2})
{
  ieh.on_create.push_back(event_t(owdemo__event_construct));
  ieh.on_delete.push_back(event_t(owdemo__event_destruct));
  clone_identifier = owdemo__id_well;
}

well_t::well_t(std::istream &is) : structure_t(is)
{
  clone_identifier = owdemo__id_well;
}

well_t::~well_t()
{
}
