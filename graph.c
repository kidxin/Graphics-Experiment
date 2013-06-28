#include <GL/glut.h>
#include <stdlib.h> 
#include <stdio.h>
#include <math.h>
#include "include/basic.h"
#include "include/circle.h"
#include "include/polygon.h"
#include "include/clipping.h"




void myDisplay(void)
{
    struct point end1;
    struct line l;
    
    int r,a,b;
    setcolour();
 //   GLubyte *pPixelData;
  //   pPixelData = (GLubyte*)malloc(1024);

    printf("Please enter the coordinates of end points:\n");
    glClear(GL_COLOR_BUFFER_BIT);
 //   glFlush();
    glPointSize(2);
//    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	
    glBegin(GL_POINTS);

	read_clip(&window,&pol_clip);
	clipping(&window,&pol_clip);
/*    readpolygon(&pol);
	fillpolygon(&pol,red,blue);
	readpolygon(&pol);
	fillpolygon(&pol,red,blue);
	*/
/*	l.p1=readpoint();
	l.p2=readpoint();
	drawline(l,white);  */
/*	 glReadPixels(0, 0, 2,2,
        GL_BGR_EXT,  GL_UNSIGNED_BYTE, pPixelData);
    int mi=*(pPixelData+2);
    printf("%d\n",mi);*/
    glEnd();

    glFlush();
//    while(1) r=r+1;
    glClear(GL_COLOR_BUFFER_BIT);
    printf("Please enter the center and radius of the circle:\n");
    scanf("%d%d%d",&end1.x,&end1.y,&r);
    glBegin(GL_POINTS);
	drawcircle(end1,r);	
    glEnd();
    glFlush();

    glClear(GL_COLOR_BUFFER_BIT);
    printf("Please enter the center and semimajor axis of the ellipse:\n");
    scanf("%d%d%d%d",&end1.x,&end1.y,&a,&b);
    glBegin(GL_POINTS);
	drawellipse(end1,a,b);	
    glEnd();
    glFlush();
    
    glClear(GL_COLOR_BUFFER_BIT);
    printf("Please enter the coordinates of end points:\n");
//    readpolygon();
    glBegin(GL_POINTS);
	drawellipse(end1,a,b);	
    glEnd();
    glFlush();

}
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
int main(int argc, char *argv[])
{
	fin = fopen(argv[1], "r");
	if (!fin) {
    	perror(argv[1]);
    	return 1;
    }
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE| GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Opengl Project");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(&myDisplay);
    glutMainLoop();

    return 0;

}
