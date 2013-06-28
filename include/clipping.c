#include <GL/glut.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
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

void clipping(struct poly *window,struct poly *pol)
{
	drawpolygon(pol,yellow);
	drawpolygon(window,red);
//	return ;
	struct poly win,clip,tpol;
	struct storp inter[MAX_INTERSECTION],newwin[MAX_VERTEX][MAX_INTERSECTION];
	
	int flag_win[MAX_INTERSECTION],flag_pol[MAX_INTERSECTION],num[MAX_INTERSECTION];
	memset(flag_win,0,MAX_INTERSECTION);
	memset(flag_pol,0,MAX_INTERSECTION);
	memset(num,0,MAX_INTERSECTION);
	int flag;
	int i,j,k = 0, x;
	
	for (i = 0;i < pol->ver_num; ++i)
	{
		j = 0;
		for (x =0;x < window->ver_num; ++x)
		if (interset(pol->edge[i],window->edge[x]))
		{
			inter[j].p = intersection(pol->edge[i],window->edge[x]);
//			test_point(inter[j].p);
			inter[j].pos = x;
			inter[j].dis = dis(inter[j].p,pol->vertex[i]);
			++j;
		}
//		printf("%d\n" , j);
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
		if (in_poly(window,pol->vertex[0])) fillpolygon(pol,white,blue);
		drawpolygon(pol,yellow);
		drawpolygon(window,red);
		return ;
	}
	tpol.ver_num = k;
	
/*	
	printf("point 1 ok\n");
*/	
	for (i = 0;i < tpol.ver_num; ++i) test_point(tpol.vertex[i]);
	return ;
	
	
	
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
//	printf("%d\n" , tpol.ver_num ) ;
//	win.ver_num = k;
//	 for (i = 0;i < win.ver_num; ++i)test_point(win.vertex[i]);
//	return ;
	makeline(&tpol);
	makeline(&win);
	int mark[MAX_INTERSECTION];
	memset(mark,0,MAX_INTERSECTION);
//	printf("%d\n" , tpol.ver_num ) ;
	while (1)
	{
		int flag = 1;
		for (i = 0;i < tpol.ver_num; ++i)
			if (flag_pol[i] == 2 && mark[i] == 0)
			{
				mark[i] = 1;
				flag=0;
				break;
			}
		if (flag) break;
		k = 0;
		while (1)
		{
//			printf("point 3 ok\n");
			
			while (flag_pol[i] != 1)
			{
				clip.vertex[k] = tpol.vertex[i];
//				test_point(clip.vertex[k]);
				++k;
				i = (i+1) % tpol.ver_num;
//				printf("%d\n" , tpol.ver_num ) ;
			}
//			printf("%d\n" , tpol.ver_num ) ; return ;
//			printf("point 4 ok\n");
			for (j = 0;j < win.ver_num; ++j)
				if (eq_point(tpol.vertex[i],win.vertex[j])) break;
//			printf("point 5 ok\n");
			while (flag_win[j] != 2)
			{
				clip.vertex[k] = win.vertex[j];
				++k;
				j = (j+1) % win.ver_num;
			}
//			printf("%d\n" , k);
           
			for (i = 0;i < tpol.ver_num; ++i)
				if (eq_point(tpol.vertex[i],win.vertex[j])) break;
			if (mark[i]) break;
			mark[i] = 1;
//			printf("point 7 ok\n");
			
		}
		clip.ver_num = k;
		printf("point 2 ok\n");
		makeline(&clip);
		fillpolygon(&clip,black,blue);
//		printf("point 1 ok\n");
	}
	printf("point 1 ok\n");
	drawpolygon(pol,yellow);
	drawpolygon(window,red);
}
