#include "basic.h"


void setcolour()
{
	black.r = 0; black.g = 0; black.b = 0;
	white.r = 1; white.g = 1; white.b = 1;
	red.r = 1; red.g = 0; red.b = 0;
	green.r = 0; green.g = 1; green.b = 0;
	blue.r = 0; blue.g = 0; blue.b = 1;
	yellow.r = 1; yellow.g = 1; yellow.b = 0;
	indigo.r = 0; indigo.g = 1; indigo.b = 1;
	magenta.r = 1; magenta.g = 0; magenta.b = 1;
}

int getred(struct point p)
{
	GLint pixel;
	glReadPixels(p.x + 300, p.y + 300, 1, 1, GL_RED, GL_INT, &pixel);
	return pixel;
}

int getgreen(struct point p)
{
	GLint pixel;
	glReadPixels(p.x + 300, p.y + 300, 1, 1, GL_GREEN, GL_INT, &pixel);
	return pixel;
}

int getblue(struct point p)
{
	GLint pixel;
	glReadPixels(p.x + 300, p.y + 300, 1, 1, GL_BLUE, GL_INT, &pixel);
	return pixel;
}

double min(double x,double y)
{
 	if (x < y) return x;
 	else return y;
}

double max(double x,double y)
{
	if (x > y) return x; 
	return y;
}

float sqr(float x)
{
    return x*x;
}

void swap(int *x,int *y)
{
    int i=*x;
    *x=*y;
    *y=i;
}

void swap_poi(struct point *p1,struct point *p2)
{
	struct point t=*p1;
	*p1=*p2;
	*p2=t;
}

void readint(int *i)
{
	fscanf(fin,"%d",i);
}

struct point readpoint()
{
	struct point p;
	readint(&p.x);
	readint(&p.y);
	return p;
}

struct line readline()
{
	struct line l;
	l.p1=readpoint();
	l.p2=readpoint();
	return l;	
}

struct plane readplane()
{
	struct plane pl;
	readint(&pl.x);
	readint(&pl.y);
	readint(&pl.z);
	readint(&pl.d);
	return pl;
}

struct colour readcolor()
{
	int i;
	readint(&i);
	switch (i)
	{
		case 0: return black;
		case 1: return white;
		case 2: return red;
		case 3: return green;
		case 4: return blue;
		case 5: return yellow;
		case 6: return indigo;
		case 7: return magenta;
		default : return black;
	}	
}

int eq_point(struct point p1,struct point p2)
{
	if (p1.x == p2.x && p2.y == p1.y) return 1;
	return 0;
}

void cal_sl(struct line *l)
{
	l->sl.dx=l->p1.x-l->p2.x;
	l->sl.dy=l->p1.y-l->p2.y;
	if (l->sl.dy<0) 
	{
		l->sl.dx = -l->sl.dx;
		l->sl.dy = -l->sl.dy;
	}
}

int delta_x(struct slope slo,int y)
{
//	int x = y*(float)slo.dx/slo.dy;
//	if (abs(x*slo.dy-slo.dx*y)>abs((x-1)*slo.dy-slo.dx*y)) --x;
//	if (abs(x*slo.dy-slo.dx*y)>abs((x+1)*slo.dy-slo.dx*y)) ++x;
	return (int)(y*(double)slo.dx/slo.dy+0.5);
}

void test_line(struct line l)
{
	printf("%d %d %d %d\n",l.p1.x,l.p1.y,l.p2.x,l.p2.y);
}

void test_point(struct point p)
{
	printf("%d %d\n",p.x,p.y);
}

int direction(struct point p0,struct point p1,struct point p2)
{
	return (p1.x - p0.x)*(p2.y - p0.y)-(p2.x - p0.x)*(p1.y - p0.y);
}

int onseg(struct point p1,struct point p2, struct point t)
{
	if (min(p1.x, p2.x) <= t.x && t.x <= max(p1.x, p2.x) && min(p1.y, p2.y) <= t.y && t.y <= max(p1.y, p2.y))
		return 1; 
	else return 0; 
}

int dis(struct point p1,struct point p2)
{
	return (int)sqr(p1.x-p2.x)+(int)sqr(p1.y-p2.y);
}

int interset(struct line l1, struct line l2)
{
	int d1,d2,d3,d4;
	d1 = direction(l2.p1, l2.p2, l1.p1);
	d2 = direction(l2.p1, l2.p2, l1.p2);
	d3 = direction(l1.p1, l1.p2, l2.p1);
	d4 = direction(l1.p1, l1.p2, l2.p2);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 >0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 >0))) return 1;
	if (d1 == 0 && onseg(l2.p1, l2.p2, l1.p1)) return 2;
	if (d2 == 0 && onseg(l2.p1, l2.p2, l1.p2)) return 3;
	if (d3 == 0 && onseg(l1.p1, l1.p2, l2.p1)) return 4;
	if (d4 == 0 && onseg(l1.p1, l1.p2, l2.p2)) return 5; 
	return 0;
}

struct point intersection(struct line l1,struct line l2)
{
	struct point u1 = l1.p1, u2 = l1.p2, v1 = l2.p1, v2 = l2.p2;
	struct point ret = u1;
	double t=(double)((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
		/((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
	ret.x += (int)((u2.x-u1.x)*t+0.5);
	ret.y += (int)((u2.y-u1.y)*t+0.5);
	return ret;
}

void drawline(struct line l,struct colour c)
{
	struct point p1=l.p1, p2=l.p2;
    int dx,dy,sx,sy,change;
    glColor3f(c.r,c.g,c.b);
    dx=abs(p1.x-p2.x);
    dy=abs(p1.y-p2.y);
    if (p2.x>p1.x) sx=1; else sx=-1;
    if (p2.y>p1.y) sy=1; else sy=-1;
    if (dy>dx) 
    {
	swap(&dx,&dy);
	change=1;
    }
    else change=0;
    int diff=2*dy-dx,i;
    for (i=0;i<dx;i++)
    {	
		glVertex2i(p1.x,p1.y);
		while (diff>0)
		{
			if (change) p1.x=p1.x+sx; else p1.y=p1.y+sy;
			diff=diff-2*dx;
		}
		if (change) p1.y=p1.y+sy; else p1.x=p1.x+sx;
		diff=diff+2*dy;
    }
}

void draw4p(struct point center,struct point tar)
{
    glVertex2i(center.x+tar.x,center.y+tar.y);
    glVertex2i(center.x-tar.x,center.y+tar.y);
    glVertex2i(center.x+tar.x,center.y-tar.y);
    glVertex2i(center.x-tar.x,center.y-tar.y);
}

void draw8p(struct point center,struct point tar)
{
    draw4p(center,tar);
    swap(&tar.x,&tar.y);
    draw4p(center,tar);
}


