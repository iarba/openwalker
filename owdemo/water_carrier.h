#ifndef OWDEMO_WATER_CARRIER_H
#define OWDEMO_WATER_CARRIER_H

#include "owdemo_main.h"

def(owdemo, id_water_carrier);
def(owdemo, nvp_water_carrier_pathfinding);
def(owdemo, tex_water_carrier);
def(owdemo, spr_water_carrier);

class water_carrier_t : public random_walker_t
{
public:
  static void init(sfml_viewer_t *view);
  static bool pathfinding_function(context_t ctx);
  class water_carrier_constructor : public abstract_walker_constructor_base
  {
    virtual walker_t *instantiate(walker_t *w);
    virtual walker_t *deserialise(std::istream &is);
  };

  static void sfml_renderer(sfml_viewer_t::viewer_context_t ctx, walker_t *w);

  water_carrier_t(glm::ivec2 position);
  water_carrier_t(std::istream &is);
  virtual ~water_carrier_t();
};

#endif // OWDEMO_WATER_CARRIER_H
