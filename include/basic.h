#ifndef __BASIC_H_
#define __BASIC_H_
#include <stdio.h>
struct point
{
    int x,y;
};

struct slope
{
	int dx,dy;
};



struct line
{
	struct point p1,p2;
	struct slope sl;
};

struct colour
{
	float r,g,b;
};

struct colour black,red,white,green,yellow,blue;

FILE *fin;

extern void setcolour();

extern void swap(int *x,int *y);

extern float sqr(float x);

extern int eq_point(struct point p1,struct point p2);

extern int dis(struct point p1,struct point p2);

extern void swap_poi(struct point *p1,struct point *p2);

extern struct point readpoint();

extern int delta_x(struct slope slo,int y);

extern void test_point(struct point p);

extern struct line readline();

extern void cal_sl(struct line *l);

extern void test_line(struct line l);

extern void drawline(struct line l,struct colour c);

extern void draw4p(struct point center,struct point tar);

extern void draw8p(struct point center,struct point tar);

extern int interset(struct line l1, struct line l2);

extern struct point intersection(struct line l1,struct line l2);


#endif
