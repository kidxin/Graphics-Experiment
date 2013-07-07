#include "include/basic.h"
#include "include/circle.h"
#include "include/polygon.h"
#include "include/clipping.h"
#include "include/shadow.h"
#include "include/curve.h"
#include <unistd.h>

int choice;

void ChangeSize(int w, int h) 
{
    GLfloat nRange = 200.0f;

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
    glPointSize(2);
//    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	ChangeSize(600,600);
	if (choice != 100) glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

	switch (choice)
	{
      	case 0:  break;
		case 1: l = readline(); c = readcolor(); drawline(l,c);
       			fclose(fin); choice = 100; break;
       			
        case 2: center = readpoint(); readint(&r); drawcircle(center,r);
       			fclose(fin); choice = 100; break;
       			
        case 3: center = readpoint(); readint(&a); readint(&b); drawellipse(center,a,b);
       			fclose(fin); choice = 100; break;
       			
        case 4:
     			fclose(fin); choice = 100; break;
     			
     	case 5: liang();
     			fclose(fin); choice = 100; break;
     			
     	case 6: read_clip(&window,&pol_clip); 
     			clipping(&window,&pol_clip);
     			fclose(fin); choice = 100; break;
     	
     	case 7: zbuffer();		
     			fclose(fin); choice = 100; break;
     
        case 9: drawcurve(); 
				fclose (fin); choice = 100;break;
       		  
        case 10: drawsurface(); 
				fclose (fin); choice = 100;break;
       		  
       default: break;
   	}
    glEnd();
    glFlush();
    
/*
    
    glClear(GL_COLOR_BUFFER_BIT);
    printf("Please enter the coordinates of end points:\n");
//    readpolygon();
    glBegin(GL_POINTS);
	drawellipse(end1,a,b);	
    glEnd();
    glFlush();
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




void mainmenu(GLint option)
{
/*	switch (option)
	{
		
	}*/
	choice = 0;
}

void blank(GLint option)
{
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE| GLUT_DEPTH);
    
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Opengl Project");
    
    choice = 0;
    
    glutReshapeFunc(ChangeSize);
    
    glutDisplayFunc(&myDisplay);
    
    GLint Basic, Clipping, Curve, Shadow;
    
    Basic = glutCreateMenu(blank);
    
    Basic = glutCreateMenu(basicgeometry);
    	glutAddMenuEntry ("Line", 1);
    	glutAddMenuEntry ("Circle", 2);
    	glutAddMenuEntry ("Ellipse", 3);
    	
    Clipping = glutCreateMenu(clip);
    	glutAddMenuEntry ("Liang", 1);
    	glutAddMenuEntry ("Weiler¨CAtherton", 2);
    
    Shadow = glutCreateMenu(shadow);
    	glutAddMenuEntry ("Zbuffer", 1);
    
    Curve = glutCreateMenu(curve);
    	glutAddMenuEntry ("Bezier Curve", 1);
    	glutAddMenuEntry ("Bezier Surface", 2);
    
    glutCreateMenu(mainmenu);
    	glutAddSubMenu ("Element", Basic);
    	glutAddSubMenu ("Clipping", Clipping);
    	glutAddSubMenu ("Shadow", Shadow);
    	glutAddSubMenu ("Curve", Curve);
    	
    
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    
    
    
    glutMainLoop();

    return 0;

}
