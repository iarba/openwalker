#ifndef OWDEMO_COMMON_PROP_H
#define OWDEMO_COMMON_PROP_H

#include "owdemo_main.h"

def(owdemo, property_road_walkable);
def(owdemo, property_house_water);
def(owdemo, property_cell_occupied);
def(owdemo, event_construct);
def(owdemo, event_destruct);

namespace common_prop
{
  void init(sfml_viewer_t *view);
  void on_construct(context_t ctx);
  void on_destruct(context_t ctx);
};

#endif // OWDEMO_COMMON_PROP_H
