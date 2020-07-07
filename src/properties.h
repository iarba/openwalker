#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>

typedef int oid_t;

typedef std::pair<int, int> namer_t;

typedef void* value_t;

typedef std::map<namer_t, value_t> properties_t;

inline static int zone_allocator;

#define zone_to_zalloc(zone) zone_allocator_ ## zone
#define zone_and_namer_to_namer(zone, namer) zone ## _ ## namer
#define def_zone(zone) inline static int zone = zone_allocator++; inline static int zone_to_zalloc(zone);
#define def(zone, namer) inline static const namer_t zone_and_namer_to_namer(zone, namer) = namer_t(zone, ++zone_to_zalloc(zone));

def_zone(z_example);
def(z_example, foo);
def(z_example, bar);

#endif // PROPERTIES_H
