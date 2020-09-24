#include "menu.h"

menu_item_t::menu_item_t(std::function<void(menu_t *)> on_click)
{
  this->on_click = on_click;
  id = menu__simple;
}

menu_item_t::~menu_item_t()
{
}

cell_menu_item_t::cell_menu_item_t(cell_t *c, std::function<void(menu_t *)> on_click) : menu_item_t(on_click)
{
  this->c = new cell_t();;
  c->copy_into(this->c);
  id = menu__simple_cell;
}

cell_menu_item_t::~cell_menu_item_t()
{
  delete this->c;
}

structure_menu_item_t::structure_menu_item_t(structure_t *s, std::function<void(menu_t *)> on_click) : menu_item_t(on_click)
{
  this->s = cloner_t::g_cloner_get()->create_structure(s);
  id = menu__simple_structure;
}

structure_menu_item_t::~structure_menu_item_t()
{
  delete this->s;
}

walker_menu_item_t::walker_menu_item_t(walker_t *w, std::function<void(menu_t *)> on_click) : menu_item_t(on_click)
{
  this->w = cloner_t::g_cloner_get()->create_walker(w);
  id = menu__simple_walker;
}

walker_menu_item_t::~walker_menu_item_t()
{
  delete this->w;
}

void menu_t::load()
{
  imp_zone(menu);
  imp(menu, simple);
  imp(menu, simple_cell);
  imp(menu, simple_structure);
  imp(menu, simple_walker);
}

menu_t::menu_t()
{
}

menu_t::~menu_t()
{
  clear_erase();
}

void menu_t::clear()
{
  items.clear();
}

void menu_t::clear_erase()
{
  for(auto it : items)
  {
    delete it;
  }
  clear();
}
