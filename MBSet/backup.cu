/* 
 * File:   MBSet.cu
 * 
 * Created on June 24, 2012
 * 
 * Purpose:  This program displays Mandelbrot set using the GPU via CUDA and
 * OpenGL immediate mode.
 * 
 */

#include <iostream>
#include <stack>
#include <cuda_runtime_api.h>
#include <stdio.h>
#include "Complex.cu"

#include <GL/freeglut.h>

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "complex.h"


// Size of window in pixels, both width and height
#define WINDOW_DIM            512

using namespace std;
int width=512, height=512, SCALE=1, xt=0,yt=0;
std::vector<Vec2D> points;
std::vector< std::vector<double> > colorss;


// Initial screen coordinates, both host and device.
Complex minC(-2.0, -1.2);
Complex maxC(1.0, 1.8);
Complex* dev_minC;
Complex* dev_maxC;
const int maxIt = 2000; // Msximum Iterations
double updateRate =20;

// Define the RGB Class
class RGB
{
public:
  RGB()
    : r(0), g(0), b(0) {}
  RGB(double r0, double g0, double b0)
    : r(r0), g(g0), b(b0) {}
public:
  double r;
  double g;
  double b;
};

RGB* colors = 0; // Array of color values

void InitializeColors()
{
  colors = new RGB[maxIt + 1];
  for (int i = 0; i < maxIt; ++i)
    {
      if (i < 5)
        { // Try this.. just white for small it counts
          colors[i] = RGB(1, 1, 1);
        }
      else
        {
          colors[i] = RGB(drand48(), drand48(), drand48());
        }
    }
  colors[maxIt] = RGB(); // black
}

void computePoints(unsigned int itt)
{
	colorss.clear();
    points.clear();
    ComplexNumber z(0,0), c(0,0);
	unsigned int ittcount;

    for ( double x = -2; x <= 1; x+=0.0059 )
	{
		for ( double y = -1.2; y <= 1.8; y+= 0.0059 )
		{
			c.real=x;c.img=y;
			z.real=0;z.img=0;
			ittcount=0;
			while ( ittcount < itt && z.length() < 4 )
			{
				z = z.multiply(z).add(c);
				ittcount++;
			}
			points.push_back(Vec2D(x,y));
			std::vector<double> color;
			color.push_back(ittcount/itt);
			color.push_back(sin((double)ittcount));
			color.push_back(abs(cos((double)ittcount)));
			colorss.push_back(color);
		}
	}
}

void setup()
{
	// we gonna do this in ortho
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2., width/2., -height/2., height/2., -1.,1);
	glMatrixMode(GL_MODELVIEW);
}

void resize(int w, int h)
{
	width=w;
	height=h;
	//setup();
	//computePoints(1000);
	setup();
}
void drawfractal()
{
	glBegin(GL_POINTS);
	for ( int i = 0; i < points.size(); i++ ){
		glColor3f(colorss[i][0], colorss[i][1], colorss[i][2]);
		glVertex2f(points[i].x*SCALE,points[i].y*SCALE);
	}
	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.,1.,1.,1);
	glLoadIdentity();
	glTranslatef(xt,-140+yt,0);
	drawfractal();
	glutSwapBuffers();
}
void mouse(int x, int y)
{
}
void mousefunc(int button, int state, int x, int y)
{
}
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'w':
		yt+=10;
		break;
	case 's':
		yt-=10;
		break;
	case 'a':
		xt-=10;
		break;
	case 'd':
		xt+=10;
		break;
	case 'z':
	case 'Z':
		SCALE*=2;
		break;
	case 'x':
	case 'X':
		SCALE/=2;
		break;
	}
}
void update(int value)
{
	glutPostRedisplay();
	glutTimerFunc(15, update, 0);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	computePoints(2000);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width,height);
	glutCreateWindow("MBSet");
	setup();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutPassiveMotionFunc(mouse);
	glutMouseFunc(mousefunc);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(25, update, 0);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(1);
	glutMainLoop();
    return 0;
}
