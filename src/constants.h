#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

#ifndef UNUSED_SYM
    #define UNUSED_SYM(x) (void)(x)
#endif

const int DISPL_WIDTH = 640;
const int DISPL_HEIGHT = 480;
const int DISPL_SCALE = 2;
const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 9;

const double tick_interval = 0.01;

#endif