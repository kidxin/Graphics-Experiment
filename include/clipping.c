#include <string.h>
#include "clipping.h"

struct storp
{
	struct point p;
	int dis,flag,pos;
};
	
void read_clip(struct poly *window,struct poly *pol)
{
	readpolygon(window);
	readpolygon(pol);
}

int comp_dis(const void *a,const void *b)
{
	return ((struct storp *)a)->dis-((struct storp *)b)->dis;
}



double u1, u2;

void range(double t1,double t2,int d)
{
	if (d == 0) return;
	if (d < 0)
	{
		u1 = max(t2/d,u1);
		u2 = min(t1/d,u2);
	}else
	{
		u1 = max(t1/d,u1);
		u2 = min(t2/d,u2);
	}
}

void drawliangwindow()
{
	struct line l;
	l.p1.x = liangx1; l.p1.y = liangy1;
	l.p2.x = liangx1; l.p2.y = liangy2;
	drawline(l,red);
	l.p2.x = liangx2; l.p2.y = liangy1;
	drawline(l,red);
	l.p1.x = liangx2; l.p1.y = liangy2;
	drawline(l,red);
	l.p2.x = liangx1; l.p2.y = liangy2;
	drawline(l,red);
}

void liang(struct line l)
{
	int dx = l.p2.x - l.p1.x;
	int dy = l.p2.y - l.p1.y;
	u1 = 0;
 	u2 = 1;
	range(liangx1-l.p1.x,liangx2-l.p1.x,dx);
	range(liangy1-l.p1.y,liangy2-l.p1.y,dy);
//	printf("%lf %lf\n",u1,u2 );
	if (u1 < u2)
	{
		struct line lt;
		lt = l;
		if (u1 > 0) 
		{
			lt.p2.x = (int)(l.p1.x + u1*dx+0.5);
			lt.p2.y = (int)(l.p1.y + u1*dy+0.5);
			drawline(lt,white);
		}
		if (u2 < 1)
		{
			lt.p2 = l.p2;
			lt.p1.x = (int)(l.p1.x + u2*dx + 0.5);
			lt.p1.y = (int)(l.p1.y + u2*dy + 0.5);
			drawline(lt,white);
		}
		lt.p1.x = (int)(l.p1.x + u1*dx + 0.5);
		lt.p2.x = (int)(l.p1.x + u2*dx + 0.5);
		lt.p1.y = (int)(l.p1.y + u1*dy + 0.5);
		lt.p2.y = (int)(l.p1.y + u2*dy + 0.5);
		drawline(lt,blue);
	}
	else  drawline(l,white);
	drawliangwindow();
}


void clipping(struct poly *window,struct poly *pol)
{
	struct poly win,clip,tpol;
	struct storp inter[MAX_INTERSECTION],newwin[MAX_VERTEX][MAX_INTERSECTION];
	
	int flag_win[MAX_INTERSECTION],flag_pol[MAX_INTERSECTION],num[MAX_INTERSECTION];
	memset(flag_win,0,sizeof(flag_win));
	memset(flag_pol,0,sizeof(flag_pol));
	memset(num,0,sizeof(num));
	int flag;
	int i,j,k = 0, x;
	
	for (i = 0;i < pol->ver_num; ++i)
	{
		j = 0;
		for (x =0;x < window->ver_num; ++x)
		if (interset(pol->edge[i],window->edge[x]))
		{
			inter[j].p = intersection(pol->edge[i],window->edge[x]);
			inter[j].pos = x;
			inter[j].dis = dis(inter[j].p,pol->vertex[i]);
			++j;
		}
		if (j) qsort(inter,j,sizeof(struct storp),comp_dis);
		int t=in_poly(window,pol->vertex[i]);
		tpol.vertex[k]=pol->vertex[i];
		++k;
		for (x = 0;x < j;++x)
		{
			tpol.vertex[k] = inter[x].p;
			flag_pol[k] = 2-((t+x) & 1);
			inter[x].flag = flag_pol[k];
			++k;
			int p=inter[x].pos;
			newwin[p][num[p]] = inter[x];
			++num[p];
		}
	}
	if (k == pol->ver_num)
	{
		if (in_poly(window,pol->vertex[0])) fillpolygon(pol,black,blue);
		drawpolygon(pol,yellow);
		drawpolygon(window,red);
		return ;
	}
	tpol.ver_num = k;	
	k = 0;
	for (i = 0;i < window->ver_num; ++i)
	{
		win.vertex[k] = window->vertex[i];
		++k;
		for (x = 0;x < num[i];++x) newwin[i][x].dis=dis(newwin[i][x].p,window->vertex[i]);
		if (num[i]) qsort(newwin[i],num[i],sizeof(struct storp),comp_dis);
		for (x = 0;x < num[i];++x)
		{
			win.vertex[k] = newwin[i][x].p;
			flag_win[k] = newwin[i][x].flag;
			++k;
		}
	}
	win.ver_num = k;
	makeline(&tpol);
	makeline(&win);
	int mark[MAX_INTERSECTION];
	memset(mark,0,sizeof(mark));
	while (1)
	{
		int flag = 1;
		for (i = 0;i < tpol.ver_num; ++i)
			if (flag_pol[i] == 2 && mark[i] == 0)
			{
				mark[i] = 1;
				flag = 0;
				break;
			}
		if (flag) break;
		k = 0;
		while (1)
		{
			while (flag_pol[i] != 1)
			{
				clip.vertex[k] = tpol.vertex[i];
//				test_point(clip.vertex[k]);
				++k;
				i = (i+1) % tpol.ver_num;
			}
			
			
			for (j = 0;j < win.ver_num; ++j)
				if (eq_point(tpol.vertex[i],win.vertex[j])) break;
			while (flag_win[j] != 2)
			{
				clip.vertex[k] = win.vertex[j];
//				test_point(clip.vertex[k]);
				++k;
				j = (j+1) % win.ver_num;
			}
			for (i = 0;i < tpol.ver_num; ++i)
				if (eq_point(tpol.vertex[i],win.vertex[j])) break;	
			if (mark[i]) break;
			mark[i] = 1;
			
		}
		clip.ver_num = k;
		makeline(&clip);
		fillpolygon(&clip,black,blue);
	}
	drawpolygon(pol,yellow);
	drawpolygon(window,red);
}
