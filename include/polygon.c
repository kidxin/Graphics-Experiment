#include <GL/glut.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include "polygon.h"



int comp_slope(const void *a,const void *b)
{
	int t1 = ((struct slope *)a)->dx * ((struct slope *)b)->dy;
	int t2 = ((struct slope *)a)->dy * ((struct slope *)b)->dx;
	int t3 = ((struct slope *)a)->dx * ((struct slope *)b)->dx;
	if (t3 <= 0) return ((struct slope *)a)->dx - ((struct slope *)b)->dx;
	else return t2-t1;
}

void makeline(struct poly *pol)
{
	int i;
	for (i=0;i < pol->ver_num;++i)
	{
		pol->edge[i].p1 = pol->vertex[i];
		pol->edge[i].p2 = pol->vertex[(i+1)%pol->ver_num];
	}
}

void nomalize(struct line *l)
{
	if (l->p1.y > l->p2.y || (l->p1.y == l->p2.y && l->p1.x > l->p2.x)) 
			swap_poi(&l->p1,&l->p2);
		cal_sl(l);
}

void readpolygon(struct poly *pol)
{
	fscanf(fin,"%d",&pol->ver_num);
	int i;
	for (i=0;i < pol->ver_num;++i)  pol->vertex[i]=readpoint();
	makeline(pol);
}

void drawpolygon(struct poly *pol,struct colour c)
{
	int i;
	for (i=0;i < pol->ver_num; ++i)
	{
		drawline(pol->edge[i],c);
	}
}

int comp_line(const void *a,const void *b)
{
	int t1 = ((struct line *)a)->p1.y-((struct line *)b)->p1.y;
	int t2 = ((struct line *)a)->p1.x-((struct line *)b)->p1.x;
	if (t1 != 0) return t1;
	else if (t2 != 0) return t2;
	else return comp_slope(&((struct line *)a)->sl,&((struct line *)b)->sl);
}

int in_poly(struct poly *pol,struct point p)
{
	struct line l;
	l.p1 = p; l.p2 = p;
	l.p2.x += 401;
	l.p2.y += 1;
	int i,j = 0;
	for (i = 0;i < pol->ver_num; ++i)
		if (interset(l, pol->edge[i])) ++j;
	return j&1;
}

void fillpolygon(struct poly *temp, struct colour edge,struct colour inter)
{
	int ver_num = temp->ver_num;
	struct line pol[MAX_VERTEX], sweep;
	sweep.p1.x=-200; sweep.p2.x=200;
	int t;
	for (t = 0;t < ver_num; ++t) 
	{
		pol[t]=temp->edge[t];
		nomalize(&pol[t]);
	}
	qsort(pol,ver_num,sizeof(struct line),comp_line);
//	int t;
//	for (t=0;t<ver_num;++t) test_line(pol[t]);
	int low,i = 0;
	struct tempor
	{
		struct line l;
		int x;
		struct tempor *next, *pre;
	};
	struct tempor *line_link, *tail, *liberty;
//	if (line_link!= NULL) printf("ok");
//	for (i=0;i<ver_num;++i) printf("%d %d\n",pol[i].p1.x,pol[i].p1.y);
	low = pol[0].p1.y+1;
	line_link = NULL;
	while (pol[i].p1.y<low) 
	{
		if (pol[i].p2.y<low) {++i; continue;}
		struct tempor *lt = (struct tempor *)malloc(sizeof(struct tempor));
		lt->l=pol[i];
		lt->pre = NULL;
		lt->next = NULL;
		if (line_link == NULL) 
		{
			line_link = lt;
			tail=lt;
		}
		else 
		{
			tail->next=lt;
			lt->pre=tail;
			tail=lt;
		}
		++i;
	}
	
	while (line_link != NULL)
	{
		tail=line_link;
//		test_line(tail->l);
//		test_line(tail->next->l);
		while (tail != NULL)
		{
			sweep.p1.y = low; sweep.p2.y = low;
			struct point t = intersection(sweep,tail->l);
		//	printf("%d %d\n",t.x,t.y);
		//	tail->x=tail->l.p1.x+delta_x(tail->l.sl,low-tail->l.p1.y);
			tail->x = t.x;
			tail=tail->next;
		}
		glColor3f(inter.r,inter.g,inter.b);
		tail = line_link;   
	//	printf("OK-----1\n");
		while (tail != NULL)
		{
			int t;
			for (t = tail->x; t <= tail->next->x; ++t) glVertex2i(t,low);
			
			tail = tail->next->next;
			
		}
	//	printf("2-----OK\n");
		++low;
		tail = line_link;
		
		while (i<ver_num && pol[i].p1.y<low) 
		{
			if (pol[i].p2.y<low) {++i; continue;}
			struct tempor *lt = (struct tempor *)malloc(sizeof(struct tempor));
			lt->l = pol[i];
			while (tail->next != NULL && tail->next->x <= pol[i].p1.x) tail = tail->next;
			lt->next = tail->next;
			if (tail->next != NULL) tail->next->pre=lt;
			lt->pre = tail;
			tail->next = lt;
			tail = lt;
	//		lt = line_link;
	//		while (lt != NULL) {test_line(lt->l); lt = lt->next;}
			++i;
		}
		
	//	printf("OK-----1\n");
		tail = line_link;
		while (tail != NULL)
		{
			
			if (tail->l.p2.y < low)
			{
				liberty = tail;
				if (tail == line_link)  line_link = tail->next;
				else   
				{
		//			test_line(tail->pre->l);
		//			printf("OK-----1\n");
					tail->pre->next = tail->next;
					if (tail->next != NULL) tail->next->pre=tail->pre;
				}
				tail = tail->next;		
				free(liberty);
			}
			else tail = tail->next;
			
		}
	//	printf("2-----OK\n");
		
	}
	drawpolygon(temp,edge);

}


