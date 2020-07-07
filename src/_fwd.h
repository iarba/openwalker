#ifndef _FWD_H
#define _FWD_H

class master_t;

class slave_t;

typedef int oid_t;

class context_t;

class world_t; //----------
// A 1                 
// |                     
// V n                   
class grid_t; //------*----  
// |                  |  
// |                  |  
// V n                |  
class structure_t; //-+----
//                    V n  
class walker_t; //---------

class master_delta;
class world_delta;
class grid_delta;
class structure_delta;
class walker_delta;

#endif // _FWD_H
