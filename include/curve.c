#include "curve.h"
#include <unistd.h>

struct point control[MAX_CONTROL];
struct point con2d[MAX_SUR][MAX_SUR];
int con_num,m,n,s;

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

struct pointd temp1[MAX_SUR],temp2[MAX_SUR];
struct point recur(double u,double v)
{
	int i,j,k;
	struct point p;
	for (i = 0;i < m;++i)
	{
		for (j = 0;j < n;++j) 
		{
			temp1[j].x = con2d[i][j].x;
			temp1[j].y = con2d[i][j].y;
		}
		for (j = 0;j < n;++j)
			for (k = 0;k < n-j-1;++k)
			{
				temp1[k].x = v*temp1[k].x+(1-v)*temp1[k+1].x;
				temp1[k].y = v*temp1[k].y+(1-v)*temp1[k+1].y;
			}
		temp2[i] = temp1[0];
	}
	for (i = 0;i < m;++i)
	{
		for (k = 0;k < m-i-1;++k)
			{
				temp2[k].x = u*temp2[k].x+(1-u)*temp2[k+1].x;
				temp2[k].y = u*temp2[k].y+(1-u)*temp2[k+1].y;
			}
	}
	p.x = (int)(temp2[0].x + 0.5);
	p.y = (int)(temp2[0].y + 0.5);
	return p;
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
	struct pointd origin[m][n];
	for (i = 0;i < m;++i)
	{
		for (j = 0;j < n;++j)
		{
			origin[i][j].x = con2d[i][j].x;
			origin[i][j].y = con2d[i][j].y;
		}
	}
	glColor3f(red.r,red.g,red.b);
	double u,v;
	for (u = 0;u <= 1; u+=0.015)
		for (v = 0;v <= 1; v+=0.015)
		{
			struct point p = recur(u,v);
			glVertex2i(p.x,p.y);
		}
}
