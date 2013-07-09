#ifndef __CURVE_H_
#define __CURVE_H_

#include "basic.h"
#define MAX_CONTROL 50
#define MAX_SUR 10
#define MAX_DEPTH 5

struct point control[MAX_CONTROL];
struct point con2d[MAX_SUR][MAX_SUR];
int con_num;

extern void drawcurve();

extern void drawsurface();

#endif
