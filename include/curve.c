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

/*struct pointd tempd[MAX_SUR][MAX_SUR][MAX_SUR][MAX_SUR];
void recur(struct pointd origin[m][n],int depth)
{
	int i,j,k,l;
	struct point p;
	++s;
	printf("ok %d\n",s);
	if (depth ==  MAX_DEPTH) 
	{
		printf("This is  %d begin\n",s);
		for (i = 0;i < m;++i)
			for (j = 0;j < n;++j) 
			{
				p.x = (int)(origin[i][j].x+0.5);
				p.y = (int)(origin[i][j].y+0.5);
				glVertex2i(p.x,p.y);
				test_point(p);
			}
		printf("This is  %d end\n",s);
		glEnd();
		glFlush();
		glBegin(GL_POINTS);
		return;
	}
	struct pointd or00[m][n],or01[m][n],or10[m][n],or11[m][n];
	for (i = 0;i < m;++i)
		for (j = 0;j < n;++j) tempd[0][0][i][j] = origin[i][j];


	for (l = 1;l < n;++l)
	{
		for (i = 0;i < m;++i)
			for (j = 0;j+l < n;++j)
			{
				tempd[0][l][i][j].x = (tempd[0][l-1][i][j].x+tempd[0][l-1][i][j+1].x)/2;
				tempd[0][l][i][j].y = (tempd[0][l-1][i][j].y+tempd[0][l-1][i][j+1].y)/2;
				if (i == 0 && j == 0) or00[0][l] = tempd[0][l][i][j];
				if (i == 0 && j+l == n-1) or01[0][j] = tempd[0][l][i][j];
				if (i == m-1 && j == 0) or10[i][l] = tempd[0][l][i][j];
				if (i == m-1 && j+l == n-1) or11[i][j] = tempd[0][l][i][j];
			}
	}

	for (k = 1;k < m;++k)
		for (l = 0;l < n;++l)
		{
			for (i = 0;i+k < m;++i)
				for (j = 0;j+l < n;++j)
				{
					if (k <m-1)
					{
						tempd[k][l][i][j].x = (tempd[k-1][l][i][j].x+tempd[k-1][l][i+1][j].x)/2;
						tempd[k][l][i][j].y = (tempd[k-1][l][i][j].y+tempd[k-1][l][i+1][j].y)/2;
					}
					else
					{
						tempd[k][l][i][j].x = (tempd[k][l-1][i][j].x+tempd[k][l-1][i][j+1].x)/2;
						tempd[k][l][i][j].y = (tempd[k][l-1][i][j].y+tempd[k][l-1][i][j+1].y)/2;
					}
					if (i == 0 && j == 0) or00[k][l] = tempd[k][l][i][j];
					if (i == 0 && j+l == n-1) or01[k][j] = tempd[k][l][i][j];
					if (i+k == m-1 && j == 0) or10[i][l] = tempd[k][l][i][j];
					if (i+k == m-1 && j+l == n-1) or11[i][j] = tempd[k][l][i][j];
				}
		}
	recur(or00,depth+1);
	recur(or01,depth+1);
	recur(or10,depth+1);
	recur(or11,depth+1);
}*/
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
	//		test_point(p);
		}
}
