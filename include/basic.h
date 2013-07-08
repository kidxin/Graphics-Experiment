#ifndef __BASIC_H_
#define __BASIC_H_
#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h> 
#include <math.h>
struct point
{
    int x,y;
};

struct slope
{
	int dx,dy;
};

struct plane
{
	int x,y,z,d;
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

struct colour black,red,white,green,yellow,blue,indigo,magenta;

FILE *fin;

extern void setcolour();

extern void swap(int *x,int *y);

extern float sqr(float x);

extern void readint(int *i);

extern int eq_point(struct point p1,struct point p2);

extern int dis(struct point p1,struct point p2);

extern void swap_poi(struct point *p1,struct point *p2);

extern struct point readpoint();

extern struct colour readcolor();

extern struct plane readplane();

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

extern double min(double x,double y);

extern double max(double x,double y);

#endif
