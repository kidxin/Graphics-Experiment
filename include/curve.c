#include "curve.h"
#include <unistd.h>

struct point control[MAX_CONTROL];
struct point con2d[MAX_CONTROL][MAX_CONTROL];
int con_num,m,n;

void readcontrol()
{
	readint(&con_num);
	int i;
	for (i = 0;i < con_num;++i)
		control[i]=readpoint();	
}

void read2d()
{
	readint(&m);
	readint(&n);
	int i,j;
	for (i = 0;i < m; ++i)
		for (j = 0;j < n;++j)
			con2d[i][j] = readpoint();
}

struct pointd
{
	double x,y;
};

void separate(struct pointd *origin)
{
	int i,j = 0;
	for (i = 1;i < con_num;++i)
		if (abs((origin+i)->x-(origin+i-1)->x) > 0.4 ||abs((origin+i)->y-(origin+i-1)->y) > 0.4 )
		{
			j = 1;
			break;
		}
	if (j == 0)
	{
		for (i = 0;i < con_num; ++i) glVertex2i((int)((origin+i)->x+0.5),(int)((origin+i)->y+0.5));
		glEnd();
		glFlush();
		usleep(50000);
		glBegin(GL_POINTS);
		return;
	}
	struct pointd *left = (struct pointd *)malloc(sizeof(struct pointd)*con_num);
	struct pointd *right = (struct pointd *)malloc(sizeof(struct pointd)*con_num);	
	for (i = 0;i < con_num; ++i)
	{
		*(left+i) = *origin;
		*(right+i) = *(origin+con_num-i-1);
		for (j = 0;j < con_num-i-1;++j)
		{
			(origin+j)->x = ((origin+j)->x + (origin+j+1)->x)/2;
			(origin+j)->y = ((origin+j)->y + (origin+j+1)->y)/2;
		}
	}
	separate(left);
	separate(right);
}

void drawcurve()
{
	readcontrol();
	int i;
	for (i = 1;i < con_num;++i)
	{
		struct line l;
		l.p1 = control[i];
		l.p2 = control[i-1];
		drawline(l,yellow);
	}
	glColor3f(red.r,red.g,red.b);
	struct pointd *origin = (struct pointd *)malloc(sizeof(struct pointd)*con_num);
	for (i = 0;i < con_num;++i)
	{
		(origin+i)->x = control[i].x;
		(origin+i)->y = control[i].y;
	}
	separate(origin);
}

void drawsurface()
{
	read2d();
	int i,j;
	for (i = 0;i < m;++i)
	{
		struct line l;
		for (j = 1;j < n;++j)
		{
			l.p1 = con2d[i][j];
			l.p2 = con2d[i][j-1];
			drawline(l,yellow);
		}
	}
	for (j = 0;j < n;++j)
	{
		struct line l;
		for (i = 1;i < m;++i)
		{
			l.p1 = con2d[i][j];
			l.p2 = con2d[i-1][j];
			drawline(l,yellow);
		}
	}
}
