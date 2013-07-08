#include "shadow.h"

void readshadow()
{
	readint(&pla_num);
	int i;
	for (i = 0;i < pla_num;++i)
	{
		proj_list[i].plan=readplane();
		readpolygon(&proj_list[i].proj);
		proj_list[i].c=readcolor();
	}
}

void zbuffer()
{
	readshadow();
	int i,j,k;
	for (i = -200;i <= 200;++i)
		for (j = -200;j <= 200;++j)
		{
			double z = 1e11;
			struct point p;
			struct colour c = black;
			p.x = i;
			p.y = j;
			for (k = 0;k < pla_num;++k)
			{
				if (proj_list[k].plan.z != 0)
					if (in_poly(&proj_list[k].proj,p))
					{
						struct plane pl = proj_list[k].plan;
						double t = (pl.d-pl.x*i-pl.y*j)/(double)pl.z;
						if (t >=0 && t < z)
						{
							z = t;
							c = proj_list[k].c;
						}
					}
					
			}
			glColor3f(c.r,c.g,c.b);
			glVertex2i(i,j);
		}
}


