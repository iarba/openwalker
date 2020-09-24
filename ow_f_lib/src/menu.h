#ifndef MENU_H
#define MENU_H

#include <openwalker/openwalker.h>
#include "object_viewer.h"
#include <functional>

def_zone(menu);
def(menu, simple);
def(menu, simple_cell);
def(menu, simple_structure);
def(menu, simple_walker);

class menu_t;

class menu_item_t
{
public:
  menu_item_t(std::function<void(menu_t *)> on_click);
  virtual ~menu_item_t();
  std::function<void(menu_t *)> on_click;
  namer_t id;
};

class cell_menu_item_t : public menu_item_t
{
public:
  cell_menu_item_t(cell_t *c, std::function<void(menu_t *)> on_click);
  virtual ~cell_menu_item_t();
  cell_t *c;
};

class structure_menu_item_t : public menu_item_t
{
public:
  structure_menu_item_t(structure_t *s, std::function<void(menu_t *)> on_click);
  virtual ~structure_menu_item_t();
  structure_t *s;
};

class walker_menu_item_t : public menu_item_t
{
public:
  walker_menu_item_t(walker_t *w, std::function<void(menu_t *)> on_click);
  virtual ~walker_menu_item_t();
  walker_t *w;
};

class menu_t
{
public:
  static void load();
  menu_t();
  virtual ~menu_t();
  void clear();
  void clear_erase();
  std::vector<menu_item_t *> items;
  int focus = 0;
  bool active = false;
};

#endif // MENU_H
