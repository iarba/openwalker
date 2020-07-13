#ifndef GOL_CELL_H
#define GOL_CELL_H

// import master header - this contains the base class
#include <openwalker/openwalker.h>

// these 2 functions help us define a namer, which is a strong unique identifier.
// however, we will need to implement the variable somewhere
def_zone(cell);
def(cell, cell);
def(cell, neighbors);

// derive a class from structure_t, which will do most of the work for us
class cell : public structure_t
{
public:
  // to maintain network compatibility, we need to create and register a cloner class for cell
  // again, derive from the existing class
  class cell_constructor : public abstract_structure_constructor_base
  {
  public:
    virtual structure_t *instantiate(structure_t *s);
    virtual structure_t *deserialise(std::istream &is);
  };

  // most important is to define a constructor - we need to set the clone identifier
  // also how else would you know where the cell initially is?
  cell(glm::ivec2 position);
  // we'll also need a stream constructor to implement deserialisation
  cell(std::istream &is);
  // there are a few more specific virtual functions that we don't really need to define
  // no need for destructor, we're not allocating memory
  // no need for serialisation, we do not have local variables

  // next function is important, it allows us to define our footprint on the map
  void append_influence_delta(influence_delta &id, context_t ctx) const;
};

// lastly, an initialisation function, to bind the constructor to the database.
// we can also implement the namer here
void cell_load();

#endif // GOL_CELL_H
