#include "include/basic.h"
#include "include/circle.h"
#include "include/polygon.h"
#include "include/clipping.h"
#include "include/shadow.h"
#include "include/curve.h"
#include <unistd.h>

int choice, flag;
struct point seedpoint;


void ChangeSize(int w, int h) 
{
    GLfloat nRange = 300.0f;
    
    // Prevent a divide by zero
    if(h == 0)
    h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
    glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
    glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myDisplay(void)
{
    struct point center;
    struct line l;
    struct colour c;
    int r,a,b;
    setcolour();  
//    glPointSize(1);
	ChangeSize(600,600);
	if (choice != 100 && choice != 17) glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
	switch (choice)
	{
      	case 0:  break;
		case 1: l = readline(); c = readcolor(); drawline(l,c);
       			fclose(fin); choice = 100; break;
       			
        case 2: center = readpoint(); readint(&r); c = readcolor(); drawcircle(center,r,c);
       			fclose(fin); choice = 100; break;
       			
        case 3: center = readpoint(); readint(&a); readint(&b); c = readcolor(); drawellipse(center,a,b,c);
       			fclose(fin); choice = 100; break;
       			
        case 4: readpolygon(&pol); fillpolygon(&pol,white,indigo);
     			fclose(fin); choice = 100; break;
     			
     	case 5: liang();
     			fclose(fin); choice = 100; break;
     			
     	case 6: read_clip(&window,&pol_clip); 
     			clipping(&window,&pol_clip);
     			fclose(fin); choice = 100; break;
     	
     	case 7: zbuffer();		
     			fclose(fin); choice = 100; break;
     
        case 9: drawcurve(); 
				fclose (fin); choice = 100; break;
       		  
        case 10: drawsurface(); 
				fclose (fin); choice = 100; break;
			
		case 11: readpolygon(&pol); drawpolygon(&pol,white); 
				fclose(fin); choice = 17; flag = 0; break;
		
		case 17: fillseed(seedpoint); break;
       		  
       default: break;
   	}
    glEnd();
    glFlush();
 /*   struct point p;
    int i,j;
    for (i = -5; i<10; ++i)
    	for (j =-5; j< 10;++j)
    	{	
    		p.x = i; p.y =j;
    		printf("red--%d %d-----%d\n",i,j,getred(p));
    	}
 */  		
}


void basicgeometry(GLint option)
{
	switch (option)
	{
		case 1: fin = fopen("test/line", "r"); choice = 1; break;
		case 2: fin = fopen("test/circle", "r"); choice = 2; break;
		case 3: fin = fopen("test/ellipse", "r"); choice = 3; break;
	}
	if (!fin) {
    	perror("File Does Not Exist\n");
    	return ;
    }
	glutPostRedisplay ();
}

void clip(GLint option)
{
	switch (option)
	{
		case 1: fin = fopen("test/liang", "r"); choice = 5; break;
		case 2: fin = fopen("test/clipping", "r"); choice = 6; break;
	}
	if (!fin) {
    	perror("File Does Not Exist\n");
    	return ;
    }
	glutPostRedisplay ();
}

void shadow(GLint option)
{
	switch (option)
	{
		case 1: fin = fopen("test/shadow", "r"); choice = 7; break;
	}
	if (!fin) {
    	perror("File Does Not Exist\n");
    	return ;
    }
	glutPostRedisplay ();
}

void curve(GLint option)
{
	switch (option)
	{
		case 1: fin = fopen("test/control", "r"); choice = 9; break;
		case 2: fin = fopen("test/surface", "r"); choice = 10; break;
	}
	if (!fin) {
    	perror("File Does Not Exist\n");
    	return ;
    }
	glutPostRedisplay ();
}

void fillpol(GLint option)
{
	switch (option)
	{
		case 1: fin = fopen("test/fillpolygon", "r"); choice = 4; break;
		case 2: fin = fopen("test/seed","r"); choice = 11; break;
	}
	glutPostRedisplay ();
}

void mainmenu(GLint option)
{
	glutPostRedisplay ();
}

void blank(GLint option)
{
}

void mousedeal(GLint button, GLint action,GLint xMouse,GLint yMouse)
{
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		switch (choice)
		{
			case 17: seedpoint.x = xMouse - 300; seedpoint.y = 300 -yMouse; break; 
			
			default: return;
		}
	}
	if (choice == 17) 
	{
		if (flag) glutPostRedisplay ();
		else flag = 1;
	}
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE| GLUT_DEPTH);
    
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("Graphic Experiment");
    
    choice = 0;
    
    glutReshapeFunc(ChangeSize);
    
    glutDisplayFunc(&myDisplay);
    
    GLint Basic, Clipping, Curve, Shadow, Fill;
    
    Basic = glutCreateMenu(blank);
    
    Basic = glutCreateMenu(basicgeometry);
    	glutAddMenuEntry ("Line", 1);
    	glutAddMenuEntry ("Circle", 2);
    	glutAddMenuEntry ("Ellipse", 3);
    	
    Fill = glutCreateMenu(fillpol);
    	glutAddMenuEntry ("Scan line", 1);
    	glutAddMenuEntry ("Seed", 2);
    	
    Clipping = glutCreateMenu(clip);
    	glutAddMenuEntry ("Liang", 1);
    	glutAddMenuEntry ("Weiler Atherton", 2);
    
    Shadow = glutCreateMenu(shadow);
    	glutAddMenuEntry ("Zbuffer", 1);
    
    Curve = glutCreateMenu(curve);
    	glutAddMenuEntry ("Bezier Curve", 1);
    	glutAddMenuEntry ("Bezier Surface", 2);
    
    glutCreateMenu(mainmenu);
    	glutAddSubMenu ("Element", Basic);
    	glutAddSubMenu ("Fill", Fill);
    	glutAddSubMenu ("Clipping", Clipping);
    	glutAddSubMenu ("Shadow", Shadow);
    	glutAddSubMenu ("Curve", Curve);
    	
    
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    
    glutMouseFunc (mousedeal);
    
    glutMainLoop();

    return 0;

}
