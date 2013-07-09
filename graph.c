#include "include/basic.h"
#include "include/circle.h"
#include "include/polygon.h"
#include "include/clipping.h"
#include "include/shadow.h"
#include "include/curve.h"
#include <unistd.h>

int choice, flag;
struct point mousepoint;
struct line majorline;

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
    struct colour c;
    int r,a,b;
    setcolour();  
//    glPointSize(1);
	ChangeSize(600,600);
	if (choice < 16) glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
	switch (choice)
	{
		case 1: flag = 0; choice = 18; break;
       			
        case 2: center = readpoint(); readint(&r); c = readcolor(); drawcircle(center,r,c);
       			fclose(fin); choice = 100; break;
       			
        case 3: center = readpoint(); readint(&a); readint(&b); c = readcolor(); drawellipse(center,a,b,c);
       			fclose(fin); choice = 100; break;
       			
        case 4: flag = 0; pol.ver_num = 0; choice = 22; break;
     			
     	case 5: flag = 0; choice =24; break;
     			
     	case 6: read_clip(&window,&pol_clip); 
     			clipping(&window,&pol_clip);
     			fclose(fin); choice = 100; break;
     	
     	case 7: zbuffer();		
     			fclose(fin); choice = 100; break;
     
        case 9: flag = 0; con_num = 0; choice = 20; break;
       		  
        case 10: drawsurface(); 
				fclose (fin); choice = 100; break;
			
		case 11: readpolygon(&pol); drawpolygon(&pol,white); 
				fclose(fin); flag = 0; choice = 17; break;
		
		case 17: if (flag) fillseed(mousepoint); break;
		
		case 18: if (flag) 
					{
						majorline.p1 = mousepoint;
					//	test_point(mousepoint);
						choice = 19;
					}
				break;
				
		case 19: if (flag == 2)
					{
						majorline.p2 = mousepoint; 
						drawline(majorline,green); 
						flag = 0;
						choice = 18;
					}
				 break;
		case 20: if (flag > con_num)
					{
						if (flag&1) majorline.p1 = mousepoint;
						else majorline.p2 = mousepoint;
						if (con_num) drawline(majorline,yellow);
						control[con_num] = mousepoint;
						++con_num;
					}
				break;
				
		case 21: if (flag == con_num) drawcurve(); break;
		
		case 22: if (flag > pol.ver_num)
					{
						if (flag&1) majorline.p1 = mousepoint;
						else majorline.p2 = mousepoint;
						if (pol.ver_num) drawline(majorline,white);
						pol.vertex[pol.ver_num] = mousepoint;
						++pol.ver_num;
					}
				 break;
		
		case 23: if (flag == pol.ver_num) fillpolygon(&pol,white,indigo); break;
		
		case 24: if (flag)
					{
						liangx1 = mousepoint.x;
						liangy1 = mousepoint.y;
						choice = 25;
					}
					break;
		case 25: if (flag == 2)
					{
						liangx2 = mousepoint.x;
						liangy2 = mousepoint.y;
						if (liangx1 > liangx2) swap(&liangx1,&liangx2);
						if (liangy1 > liangy2) swap(&liangy1,&liangy2);
						drawliangwindow();
						flag = 0;
						choice = 26;					
					}
					break;
       	case 26: if (flag)
       				{
       					majorline.p1 = mousepoint;
       					choice = 27;
       				}
       				break;
       	
       	case 27: if (flag == 2)
       				{
       					majorline.p2 = mousepoint;
       					liang(majorline);
       					flag = 0;
       					choice = 26;
       				}
       				break;
       	
       	
        default: break;
   	}
    glEnd();
    glFlush();	
}


void basicgeometry(GLint option)
{
	switch (option)
	{
		case 1: choice = 1; break;
		case 2: fin = fopen("test/circle", "r"); choice = 2; break;
		case 3: fin = fopen("test/ellipse", "r"); choice = 3; break;
	}
	glutPostRedisplay ();
}

void clip(GLint option)
{
	switch (option)
	{
		case 1: choice = 5; break;
		case 2: fin = fopen("test/clipping", "r"); choice = 6; break;
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
		case 1: choice = 9; break;
		case 2: fin = fopen("test/surface", "r"); choice = 10; break;
	}
	glutPostRedisplay ();
}

void fillpol(GLint option)
{
	switch (option)
	{
		case 1: choice = 4; break;
		case 2: fin = fopen("test/seed","r"); choice = 11; break;
	}
	glutPostRedisplay ();
}

void mainmenu(GLint option)
{
	return ;
}

void blank(GLint option)
{
}

void mousedeal(GLint button, GLint action,GLint xMouse,GLint yMouse)
{
	mousepoint.x = xMouse - 300; mousepoint.y = 300 -yMouse;
	if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN)
	{
		switch (choice)
		{
			case 17:  
			case 18: 
			case 19: 
			case 20: 
			case 21:
			case 22: 
			case 23:
			case 24:
			case 25:
			case 26:
			case 27: ++flag; break;
			default: return;
		}
		glutPostRedisplay ();
	}
}

void keyfun(GLubyte key, GLint xMouse, GLint yMouse)
{
	if (key == 13)
	{
		switch (choice)
		{	
			case 20: choice = 21; break;
			case 22: choice = 23; break;
			default: return;
		}
		glutPostRedisplay ();
	}
	else if (key == 27)
	{
		if (choice == 100) choice = 0;
		if (choice > 23) choice = 5;
		else if (choice > 21) choice = 4;
		else if (choice > 19) choice = 9;
		else if (choice > 17) choice = 1;
		else choice = 0;
		glutPostRedisplay ();
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
    	
    
    glutAttachMenu (GLUT_LEFT_BUTTON);
    
    glutMouseFunc (mousedeal);
    
    glutKeyboardFunc (keyfun);
    
    glutMainLoop();

    return 0;

}
