#ifndef __polygon_H_
#define __polygon_H_

#include "basic.h"
#define MAX_VERTEX 50

struct poly
{
	struct point vertex[MAX_VERTEX];
	struct line edge[MAX_VERTEX];
	int ver_num;
};

struct poly pol;


extern void readpolygon(struct poly *pol);

extern void drawpolygon(struct poly *pol,struct colour c);

extern void fillpolygon(struct poly *temp, struct colour edge,struct colour inter);

extern int in_poly(struct poly *pol,struct point p);

extern void makeline(struct poly *pol);

extern void fillseed(struct point seed);

#endif
