#include "influence.h"

influence_delta::influence_delta()
{
}

influence_delta::influence_delta(std::istream &is)
{
  int count;
  glm::ivec2 where;
  is >> count;
  while(count--)
  {
    is >> where.x >> where.y;
    is >> cell_persistent_setters[where];
  }
  is >> count;
  while(count--)
  {
    is >> where.x >> where.y;
    is >> cell_temporary_setters[where];
  }
}

void influence_delta::serialise(std::ostream &os)
{
  os << " " << cell_persistent_setters.size() << " ";
  for(auto it : cell_persistent_setters)
  {
    os << " " << it.first.x << " " << it.first.y << " " << it.second << " ";
  }
  os << " " << cell_temporary_setters.size() << " ";
  for(auto it : cell_temporary_setters)
  {
    os << " " << it.first.x << " " << it.first.y << " " << it.second << " ";
  }
}

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
  return id;
}
