/* Gatech ECE2035 2015 SPRING PAC MAN
 * Copyright (c) 2015 Gatech ECE2035
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef MAP_PRIVATE_H
#define MAP_PRIVATE_H

#include "mbed.h"

#ifndef ULCD_4DGL_H_
#define ULCD_4DGL_H_
#include "uLCD_4DGL.h"
#endif

#include "globals.h"
#include "map_public.h"

// It defines the settings for drawing the map
#define WALL_COLOR    0x0000FF
#define COOKIE_RADIUS 1
#define SUPER_COOKIE_RADIUS 2
#define COOKIE_COLOR  0x00FF00

// It defines the values used in the DEFAULT_MAP
#define MAP_ATTRIBUTE_WALL         0 //wall
#define MAP_ATTRIBUTE_COOKIE       1 //cookie
#define MAP_ATTRIBUTE_SUPER_COOKIE 2 //super cookie

#define DEFAULT_MAP {\
0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,  \
0,   2,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   0,  \
0,   1,   0,   0,   1,   0,   0,   1,   0,   1,   0,   0,   1,   0,   0,   1,   0,  \
0,   1,   0,   0,   1,   0,   0,   1,   0,   1,   0,   0,   1,   0,   0,   1,   0,  \
1,   1,   1,   1,   1,   1,   0,   1,   0,   1,   0,   1,   1,   1,   1,   1,   1,  \
0,   1,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   0,   0,   0,   1,   0,  \
0,   1,   1,   0,   0,   1,   0,   0,   1,   0,   0,   1,   0,   0,   1,   1,   0,  \
0,   0,   1,   1,   1,   1,   0,   1,   1,   1,   0,   1,   1,   1,   1,   0,   0,  \
0,   0,   1,   0,   0,   1,   0,   0,   0,   0,   0,   1,   0,   0,   1,   0,   0,  \
0,   1,   1,   0,   0,   1,   1,   1,   1,   1,   1,   1,   0,   0,   1,   1,   0,  \
0,   1,   0,   0,   0,   1,   0,   0,   1,   0,   0,   1,   0,   0,   0,   1,   0,  \
1,   1,   1,   1,   1,   2,   0,   0,   1,   0,   0,   2,   1,   1,   1,   1,   1,  \
0,   1,   0,   0,   1,   0,   0,   0,   1,   0,   0,   0,   1,   0,   0,   1,   0,  \
0,   1,   0,   0,   1,   0,   1,   1,   1,   1,   1,   0,   1,   0,   0,   1,   0,  \
0,   2,   1,   1,   1,   1,   1,   0,   0,   0,   1,   1,   1,   1,   1,   2,   0,  \
0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0}

// Here defines some useful macros
#define IDX2X(idx)  (idx%NUM_GRID_X)
#define IDX2Y(idx)  (idx/NUM_GRID_X)
#define XY2IDX(x,y) (y*NUM_GRID_X+x)

void map_draw(void);


#endif //MAP_H