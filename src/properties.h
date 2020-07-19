#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <iostream>

void properties_load();

typedef std::pair<int, int> namer_t;

typedef namer_t oid_t;

typedef intptr_t value_t;

typedef std::map<namer_t, value_t> properties_t;

inline int zone_allocator = 0;

#define domain_to_zone(domain) zone_ ## domain
#define domain_to_zalloc(domain) zone_allocator_ ## domain
#define domain_and_index_to_namer(domain, index) domain ## __ ## index

#define def_zone(zone) inline int domain_to_zone(zone); inline int domain_to_zalloc(zone);
#define def(zone, namer) inline namer_t domain_and_index_to_namer(zone, namer);

#define imp_zone(zone) domain_to_zone(zone) = zone_allocator++; domain_to_zalloc(zone) = 0;
#define imp(zone, namer) domain_and_index_to_namer(zone, namer) = namer_t(domain_to_zone(zone), ++domain_to_zalloc(zone));

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

def_zone(null);
def(null, null);

std::ostream& operator<<(std::ostream &os, const namer_t &data);

std::istream& operator>>(std::istream &is, namer_t &data);

std::ostream& operator<<(std::ostream &os, const properties_t &data);

std::istream& operator>>(std::istream &is, properties_t &data);

#endif // PROPERTIES_H
