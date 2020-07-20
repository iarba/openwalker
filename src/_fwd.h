#ifndef _FWD_H
#define _FWD_H

class event_t;

class master_t;

class slave_t;

class context_t;

class world_t; //-------------
// A 1                 
// |                     
// V n                   
class grid_t; //------*-----*-
// |                  |     |
// |                  |     |
// V n                |     |
class structure_t; //-+-----+-
//                    V n   |
class walker_t; //----------+-
//                          V size^2
class cell_t; //--------------

class master_delta;
class world_delta;
class grid_delta;
class structure_delta;
class walker_delta;

class cloner_t;
class abstract_grid_constructor_base;
class abstract_structure_constructor_base;
class abstract_walker_constructor_base;

#endif // _FWD_H
