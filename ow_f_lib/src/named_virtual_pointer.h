#ifndef NAMED_VIRTUAL_POINTER_H
#define NAMED_VIRTUAL_POINTER_H

#include <openwalker/openwalker.h>
#include <map>

class dynamic_named_pointer_manager_t
{
public:
  static dynamic_named_pointer_manager_t *g_dnpm_get();
  void set(namer_t where, void *value);
  void *get(namer_t where);
private:
  std::map<namer_t, void *> values;
};

#endif // NAMED_VIRTUAL_POINTER_H
