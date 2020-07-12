#include "test.h"

void tst_load()
{
  imp_zone(test);
  imp(test, walkable);
  imp(cloner_registry, road_cloner);
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
  clone_identifier = cloner_registry__road_cloner;
}

road::road(std::istream &is) : structure_t(is)
{
  clone_identifier = cloner_registry__road_cloner;
}

road::~road()
{
}

void road::serialise(std::ostream &os)
{
  this->structure_t::serialise(os);
}

void road::copy_into(road *other)
{
  this->structure_t::copy_into(other);
}

void road::append_influence_delta(influence_delta &id, context_t ctx) const
{
  structure_t::append_influence_delta(id, ctx);
  id.cell_persistent_setters[position][test__walkable] = (value_t)1;
}
