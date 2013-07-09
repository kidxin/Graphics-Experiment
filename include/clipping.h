#ifndef __clipping_H_
#define __clipping_H_

#include "basic.h"
#include "polygon.h"
#define MAX_INTERSECTION 50

struct poly window,pol_clip;

int liangx1,liangx2,liangy1,liangy2;

extern void read_clip(struct poly *window,struct poly *pol);

extern void clipping(struct poly *window,struct poly *pol);

extern void liang(struct line l);

extern void drawliangwindow();

#endif
