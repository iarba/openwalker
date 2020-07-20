#ifndef OWDEMO_ROAD_H
#define OWDEMO_ROAD_H

#include "owdemo_main.h"

def(owdemo, id_road);
def(owdemo, property_road_walkable);
def(owdemo, event_road_construct);
def(owdemo, event_road_destruct);
def(owdemo, tex_road);
def_vec(owdemo, spr_road_vec, 16);

class road_t : public structure_t
{
public:
  static void init(sfml_viewer_t *view);
  class road_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *s);
    virtual structure_t *deserialise(std::istream &is);
  };

  static void on_construct(context_t ctx);
  static void on_destruct(context_t ctx);

  static void sfml_renderer(sfml_viewer_t::viewer_context_t ctx, structure_t *s);

  road_t(glm::ivec2 position);
  road_t(std::istream &is);
  virtual ~road_t();
};

#endif // OWDEMO_ROAD_H
