#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <iostream>

typedef int oid_t;

typedef std::pair<int, int> namer_t;

typedef void* value_t;

typedef std::map<namer_t, value_t> properties_t;

inline static int zone_allocator = 0;

#define zone_to_zalloc(zone) zone_allocator_ ## zone
#define zone_and_namer_to_namer(zone, namer) zone ## __ ## namer

#define def_zone(zone) inline int zone; inline int zone_to_zalloc(zone);
#define def(zone, namer) inline namer_t zone_and_namer_to_namer(zone, namer);

#define imp_zone(zone) zone = zone_allocator++; zone_to_zalloc(zone) = 0;
#define imp(zone, namer) zone_and_namer_to_namer(zone, namer) = namer_t(zone, ++zone_to_zalloc(zone));

/*
def_zone(z_example);
def(z_example, foo);
def(z_example, bar);

void z_example_load()
{
  imp_zone(z_example);
  imp(z_example, foo);
  imp(z_example, bar);
}
*/

std::ostream& operator<<(std::ostream &os, const namer_t &data);

std::istream& operator>>(std::istream &is, namer_t &data);

std::ostream& operator<<(std::ostream &os, const properties_t &data);

std::istream& operator>>(std::istream &is, properties_t &data);

#endif // PROPERTIES_H
