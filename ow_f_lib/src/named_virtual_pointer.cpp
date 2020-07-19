#include "named_virtual_pointer.h"

dynamic_named_pointer_manager_t dnpm;

dynamic_named_pointer_manager_t *dynamic_named_pointer_manager_t::g_dnpm_get()
{
  return &dnpm;
}

void dynamic_named_pointer_manager_t::set(namer_t where, void *value)
{
  values[where] = value;
}

void *dynamic_named_pointer_manager_t::get(namer_t where)
{
  return values[where];
}
