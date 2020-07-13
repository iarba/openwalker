#include "influence.h"
#include "misc_utils.h"

influence_delta::influence_delta()
{
}

influence_delta::influence_delta(std::istream &is)
{
  int count;
  glm::ivec2 where;
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where.x >> where.y);
    ow_assert(is >> cell_persistent_setters[where]);
  }
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where.x >> where.y);
    ow_assert(is >> cell_temporary_setters[where]);
  }
  ow_assert(is >> count);
  while(count--)
  {
    ow_assert(is >> where.x >> where.y);
    ow_assert(is >> cell_unsetters[where]);
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
  os << " " << cell_unsetters.size() << " ";
  for(auto it : cell_unsetters)
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
  for(auto it : this->cell_unsetters)
  {
    for(auto it2 : it.second)
    {
      id.cell_unsetters[it.first][it2.first] = it2.second;
    }
  }
  return id;
}
