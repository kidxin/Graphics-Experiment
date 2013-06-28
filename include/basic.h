#ifndef __BASIC_H_
#define __BASIC_H_

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

extern void setcolour();

extern void swap(int *x,int *y);

extern void swap_poi(struct point *p1,struct point *p2);

extern struct point readpoint();

extern struct line readline();

extern void cal_sl(struct line *l);

extern int delta_x(struct slope slo,int y);

extern void test_line(struct line l);

extern void drawline(struct line l,struct colour c);

extern void draw4p(struct point center,struct point tar);

extern void draw8p(struct point center,struct point tar);




#endif