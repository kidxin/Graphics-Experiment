#include "circle.h"


void moved(struct point *now,int *diff)
{
    now->x=now->x+1;
    now->y=now->y-1;
    diff=diff+2*now->x-2*now->y+2;
}
void drawcircle(struct point center,int rad)
{
    struct point now;
    int diff;
    now.x=0;
    now.y=rad;
    diff=2*(1-rad);
    while (now.y>=now.x)
    {
		draw8p(center,now);
		if (diff<0)
			if (2*diff+2*now.y-1<=0)
			{
				now.x=now.x+1;
				diff=diff+2*now.x+1;
			}
			else
			{
				now.x=now.x+1;
				now.y=now.y-1;
				diff=diff+2*now.x-2*now.y+2;
			}
		else 
		if (diff>0)
			if (2*diff-2*now.x-1<=0) 
			{
				now.x=now.x+1;
				now.y=now.y-1;
				diff=diff+2*now.x-2*now.y+2;
			}
			else
			{
				now.y=now.y-1;
				diff=diff-2*now.y+1;
			}
		else   
		{
			now.x=now.x+1;
			now.y=now.y-1;
			diff=diff+2*now.x-2*now.y+2;
		}
    }
}

void drawellipse(struct point center,int a,int b)
{
    int aa=a*a,bb=b*b;
    float diff;
    struct point now;
    now.x=a; now.y=0;
    diff=aa-a*bb+(float)bb/4;
    while (bb*now.x>aa*now.y)
    {
	draw4p(center,now);
	now.y++;
	if (diff<0) 
	    diff+=2*aa*now.y+aa;
	else
	{
	    now.x--;
	    diff=diff-2*bb*now.x+2*aa*now.y+aa;
	}
    }
    diff=bb*sqr(now.x-1)+aa*sqr(now.y+0.5)-aa*bb;
    while (now.x>=0)
    {
	draw4p(center,now);
	now.x--;
	if (diff<0)
	{
	    now.y++;
	    diff+=2*aa*now.y-2*bb*now.x+bb;
	}
	else
	    diff=diff-2*bb*now.x+bb;
    }
}


