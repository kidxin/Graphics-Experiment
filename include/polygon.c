#include "polygon.h"

int comp_slope(const void *a,const void *b)
{
	int t1 = ((struct slope *)a)->dx * ((struct slope *)b)->dy;
	int t2 = ((struct slope *)a)->dy * ((struct slope *)b)->dx;
	int t3 = ((struct slope *)a)->dx * ((struct slope *)b)->dx;
	if (t3 <= 0) return ((struct slope *)a)->dx - ((struct slope *)b)->dx;
	else return t1-t2;
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
	readint(&pol->ver_num);
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
	l.p2.x += 601;
	l.p2.y += 1;
	int i,j = 0;
	for (i = 0;i < pol->ver_num; ++i)
		if (interset(l, pol->edge[i]) > 1) return 1;
		else if (interset(l, pol->edge[i]) == 1) ++j;
	return j&1;
}

struct plist
{
	struct point p;
	struct plist *next;
};

struct plist  *creatpl(struct plist *tail, struct point p)
{
	glBegin(GL_POINTS);
	glVertex2i(p.x,p.y);
	glEnd();
	struct plist *temp = (struct plist *)malloc(sizeof(struct plist));
	temp->p = p;
	temp->next = NULL;
	tail->next = temp;
	return temp;
}

void fillseed(struct point seed)
{
	glEnd();
	if (getred(seed)) return;
	glColor3f(1,0,0);
	struct plist *head = (struct plist *)malloc(sizeof(struct plist));
	head->p = seed;
	struct plist *tail;
	tail = head;
	while (head != NULL)
	{
		glFlush();
		struct point p;
		p = head->p;
		++p.x;
		if (getred(p) == 0) tail = creatpl(tail,p);		
		p.x -= 2;
		if (getred(p) == 0) tail = creatpl(tail,p);
		++p.x; ++p.y;
		if (getred(p) == 0) tail = creatpl(tail,p);
		p.y-=2;
		if (getred(p) == 0) tail = creatpl(tail,p);
		struct plist *t;
		t = head;
		head = head->next;		
		free(t);
		
	}
}

void fillpolygon(struct poly *temp, struct colour edge,struct colour inter)
{
	int ver_num = temp->ver_num;
	struct line pol[MAX_VERTEX];
//	sweep.p1.x=-200; sweep.p2.x=200;
	int t;
	glColor3f(inter.r,inter.g,inter.b);
	for (t = 0;t < ver_num; ++t) 
	{
		pol[t]=temp->edge[t];
		nomalize(&pol[t]);
	}
	qsort(pol,ver_num,sizeof(struct line),comp_line);
	int low,i = 0;
	struct tempor
	{
		struct line l;
		int x;
		struct tempor *next, *pre;
	};
	struct tempor *line_link, *tail, *liberty;
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
		while (tail != NULL)
		{
			tail->x=tail->l.p1.x+delta_x(tail->l.sl,low-tail->l.p1.y);
			tail=tail->next;
		}
		tail = line_link;   
		while (tail != NULL)
		{
			int t;
			for (t = tail->x; t < tail->next->x; ++t) glVertex2i(t,low);
			
			tail = tail->next->next;
			
		}
		++low;
		tail = line_link;
		
		while (i<ver_num && pol[i].p1.y<low) 
		{
			if (pol[i].p2.y<low) {++i; continue;}
			struct tempor *lt = (struct tempor *)malloc(sizeof(struct tempor));
			lt->l = pol[i];
			lt->x = pol[i].p1.x;
			if (lt->x < line_link->x)
			{
				lt->next = line_link;
				line_link->pre = lt;
				tail = lt;	
				line_link = lt;
				++i;
				continue;
			}
			while (tail->next != NULL && tail->next->x <= pol[i].p1.x) tail = tail->next;
			lt->next = tail->next;
			if (tail->next != NULL) tail->next->pre=lt;
			lt->pre = tail;
			tail->next = lt;
			tail = lt;
			++i;
		}
		tail = line_link;
		while (tail != NULL)
		{
			
			if (tail->l.p2.y < low)
			{
				liberty = tail;
				if (tail == line_link)  line_link = tail->next;
				else   
				{
					tail->pre->next = tail->next;
					if (tail->next != NULL) tail->next->pre=tail->pre;
				}
				tail = tail->next;		
				free(liberty);
			}
			else tail = tail->next;
			
		}	
	}
	drawpolygon(temp,edge);

}


