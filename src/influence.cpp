#include "influence.h"

influence_delta influence_delta::operator+(const influence_delta &other)
{
  influence_delta id = other;
  for(auto it : this->cell_persistent_setters)
  {
    for(auto it2 : it.second)
    {
      id.cell_persistent_setters[it.first][it2.first] = it2.second;
    }
  }
  for(auto it : this->cell_temporary_setters)
  {
    for(auto it2 : it.second)
    {
      id.cell_temporary_setters[it.first][it2.first] = it2.second;
    }
  }
}
