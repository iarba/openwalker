#include "properties.h"
#include "misc_utils.h"

void properties_load()
{
  imp_zone(null);
  imp(null, null);
}

std::ostream& operator<<(std::ostream &os, const namer_t &data)
{
  os << data.first << " " << data.second;
  return os;
}

std::istream& operator>>(std::istream &is, namer_t &data)
{
  ow_assert(is >> data.first >> data.second);
  return is;
}

std::ostream& operator<<(std::ostream &os, const properties_t &data)
{
  os << data.size();
  for(auto it : data)
  {
    os << " " << it.first << " " << it.second;
  }
  return os;
}

std::istream& operator>>(std::istream &is, properties_t &data)
{
  int count;
  ow_assert(is >> count);
  while(count--)
  {
    namer_t n;
    value_t v;
    ow_assert(is >> n >> v);
    data[n] = v;
  }
  return is;
}
