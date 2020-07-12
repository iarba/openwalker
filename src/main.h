#ifndef MAIN_H
#define MAIN_H

#include "_fwd.h"
#include "context.h"
#include "influence.h"
#include "network.h"
#include "master.h"
#include "clone.h"
#include "world.h"
#include "grid.h"
#include "structure.h"
#include "walker.h"
#include "misc_utils.h"

extern master_t *openwalker_master;

void openwalker_init(int seed);

void openwalker_destroy();

#endif // MAIN_H
