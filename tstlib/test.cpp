#include "test.h"

void road_setter(context_t ctx)
{
  grid_t *g = ctx.world->get_grid(ctx.grid_id);
  structure_t *s = g->get_structure(ctx.element_id);
  g->at(s->get_position())->set_persistent(test__walkable, (value_t)1);
}

void road_unsetter(context_t ctx)
{
  grid_t *g = ctx.world->get_grid(ctx.grid_id);
  structure_t *s = g->get_structure(ctx.element_id);
  g->at(s->get_position())->unset(test__walkable);
}

void master_pause_toggler(context_t ctx)
{
  ctx.world->dirt->set(master_config__autorun, (value_t) !ctx.world->dirt->get(master_config__autorun));
  ((master_t *)ctx.world)->update();
}

void tst_load()
{
  imp_zone(test);
  imp(test, walkable);
  imp(cloner_registry, road_cloner);
  imp(test, ev_road_set);
  imp(test, ev_road_unset);
  event_register(test__ev_road_set, ow_event_helpers::ret_0, road_setter);
  event_register(test__ev_road_unset, ow_event_helpers::ret_0, road_unsetter);
  event_register(test__ev_toggle_pause, ow_event_helpers::ret_0, master_pause_toggler);
  cloner_t::g_cloner_get()->reg_structure(cloner_registry__road_cloner, new road::road_constructor());
}

structure_t *road::road_constructor::instantiate(structure_t *s)
{
  road *r = (road *)s;
  road *nr = new road(r->position);
  r->copy_into(nr);
  return nr;
}

structure_t *road::road_constructor::deserialise(std::istream &is)
{
  road *nr = new road(is);
  return nr;
}

road::road(glm::ivec2 position) : structure_t(position)
{
  ieh.on_create.push_back(event_t(test__ev_road_set));
  ieh.on_delete.push_back(event_t(test__ev_road_unset));
  clone_identifier = cloner_registry__road_cloner;
}

road::road(std::istream &is) : structure_t(is)
{
  clone_identifier = cloner_registry__road_cloner;
}

road::~road()
{
}

void road::serialise(std::ostream &os) const
{
  this->structure_t::serialise(os);
}

void road::copy_into(road *other) const
{
  this->structure_t::copy_into(other);
}

void road::append_influence_delta(influence_delta &id, context_t ctx) const
{
  structure_t::append_influence_delta(id, ctx);
}
