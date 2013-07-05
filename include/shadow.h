#ifndef __SHADOW_H_
#define __SHADOW_H_
#include "polygon.h"
#define MAX_PLANE_NUMBER 50

struct  projection
{
	struct plane plan; 
	struct poly proj;
	struct colour c;
};

struct projection proj_list[MAX_PLANE_NUMBER];

int pla_num;

extern void zbuffer();

#endif
