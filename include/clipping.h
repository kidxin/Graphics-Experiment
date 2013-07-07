#ifndef __clipping_H_
#define __clipping_H_

#include "basic.h"
#include "polygon.h"
#define MAX_INTERSECTION 50

struct poly window,pol_clip;

extern void read_clip(struct poly *window,struct poly *pol);

extern void clipping(struct poly *window,struct poly *pol);

extern void liang();


#endif
