#ifndef OWDEMO_WELL_H
#define OWDEMO_WELL_H

#include "owdemo_main.h"

def(owdemo, id_well);
def(owdemo, tex_well);
def(owdemo, spr_well);
def(owdemo, ev_well_spawn);

class well_t : public structure_t
{
public:
  static void init(sfml_viewer_t *view);
  class well_constructor : public abstract_structure_constructor_base
  {
    virtual structure_t *instantiate(structure_t *s);
    virtual structure_t *deserialise(std::istream &is);
  };

  static void sfml_renderer(sfml_viewer_t::viewer_context_t ctx, structure_t *s);

  well_t(glm::ivec2 position);
  well_t(std::istream &is);
  virtual void serialise(std::ostream &os) const;
  virtual ~well_t();

  int spawn_cd = 100;
};

#endif // OWDEMO_ROAD_H
