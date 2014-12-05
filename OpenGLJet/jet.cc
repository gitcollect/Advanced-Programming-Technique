// OpenGL Jet program
// Qichao Chu
// ECE8893, Georgia Tech

#include <iostream>

#ifdef LINUX
//Linux headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool   rotatexaxis = false;
bool   rotateyaxis = false;
bool   rotatezaxis = false;
bool   light = false;
int    xangle = 0; 
int    yangle = 0;
int    zangle = 0;
double sizefactor = 1.0;

struct Vertex {
	GLfloat x, y, z;
};

class Material {
    public:
	GLfloat sr, sg, sb;
	GLfloat ar, ag, ab;
	GLfloat dr, dg, db;
	GLfloat er, eg, eb;
	GLfloat transparency;
	GLfloat shiny;
	int n;								
};

vector<Material> materials;
vector<Vertex> vertex, normal, fa, fb, fc, fna, fnb, fnc;
vector<int> mi;

double updateRate =20;
// code to read the obj file
void Read() {
    
    ifstream ifs("jet.obj");
    int cms = 0;
    while(ifs) {
        string part;
        ifs >> part;
        struct Vertex temp;
        if (part == "v") {
            ifs >> temp.x >> temp.y >> temp.z;
    		vertex.push_back(temp);
        }
        else if (part == "vn") {
    	  	ifs >> temp.x >> temp.y >> temp.z;
    		normal.push_back(temp);
        }
        else if(part == "usemtl") {
    	  	ifs >> cms;	
        }
        else if(part == "f") {
        	string f[3];
        	int vi[3], fi[3], ni[3];
        	ifs >> f[0] >> f[1] >> f[2];
        	for(int i = 0; i < 3; i++) {
        		sscanf((f[i].c_str()), "%i/%i/%i", &vi[i], &fi[i], &ni[i]);
        	}
        	fa.push_back(vertex[vi[0] - 1]);
            fb.push_back(vertex[vi[1] - 1]); 
            fc.push_back(vertex[vi[2] - 1]);
            fna.push_back(normal[ni[0] - 1]);
            fnb.push_back(normal[ni[1] - 1]);
            fnc.push_back(normal[ni[2] - 1]);
			mi.push_back(cms);
        }
    }
}

// Code to read material file
void ReadMtl()
{
    ifstream ifs("jet.mtl");
    int i;
    while(ifs)
    {
        string part;
        ifs >> part;
        if (part == "newmtl") {
          	ifs >> i;
			materials.push_back(Material());
		    materials[i].n = i;
        }
else if (part == "Ns") {
          	ifs >> materials[i].shiny;
        }
        else if (part == "Tr") {
		    ifs >> materials[i].transparency;
        }
	    else if (part == "Ka") {
		    ifs >> materials[i].ar
		    	>> materials[i].ag
		    	>> materials[i].ab;
	    }
	    else if (part == "Kd") {
		    ifs >> materials[i].dr
		    	>> materials[i].dg
		    	>> materials[i].db;
	    }
	    else if(part == "Ks") {
		    ifs >> materials[i].sr
		        >> materials[i].sg
		        >> materials[i].sb;
    	}
	    else if(part == "Ke") {
    		ifs >> materials[i].er
    			>> materials[i].eg
    			>> materials[i].eb;
	    }
    }
}

void setMaterial(int materialId)
{
    float specular[4];
    float ambient[4];
    float diffuse[4];
    float emmisive[4];
    float shiny;
    
    // Ugly but works
    specular[0] = materials[materialId].sr;
    specular[1] = materials[materialId].sg;
    specular[2] = materials[materialId].sb;
    specular[3] = 1 - materials[materialId].transparency;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    
    ambient[0] = materials[materialId].ar;
    ambient[1] = materials[materialId].ag;
    ambient[2] = materials[materialId].ab;
    ambient[3] = 1 - materials[materialId].transparency;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    
    diffuse[0] = materials[materialId].dr;
    diffuse[1] = materials[materialId].dg;
    diffuse[2] = materials[materialId].db;
    diffuse[3] = 1 - materials[materialId].transparency;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    
    emmisive[0] = materials[materialId].er;
    emmisive[1] = materials[materialId].eg;
    emmisive[2] = materials[materialId].eb;
    emmisive[3] = 1 - materials[materialId].transparency;
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmisive);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[materialId].shiny);
}

void drawModel()
{
    for(unsigned int i = 0; i < mi.size(); i++) {
    	
    	glBegin(GL_TRIANGLES);
    	setMaterial(mi[i]);

    	glVertex3f(fa[i].x, fa[i].y, fa[i].z);
    	glVertex3f(fb[i].x, fb[i].y, fb[i].z);
    	glVertex3f(fc[i].x, fc[i].y, fc[i].z); 

    	glNormal3f(fna[i].x, fna[i].y, fna[i].z);
    	glNormal3f(fnb[i].x, fnb[i].y, fnb[i].z);
    	glNormal3f(fnc[i].x, fnc[i].y, fnc[i].z);
    	
    	glEnd();
    }
    // Your code to draw the model here
}

void init(void)
{ // Called from main
  	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_BLEND);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void timer(int)
{
  	// Adjust rotation angles as needed here
  	// Then tell glut to redisplay
  	glutPostRedisplay();
  	// And reset tht timer
  	glutTimerFunc(1000 / updateRate, timer, 0);
}

void display(void)
{
   	if(light) {
    	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    	GLfloat LightDiffuse[]= { 1.0f, 0.0f, 1.0f, 1.0f }; 
    	GLfloat LightPosition[]= { 1000.0f, 1000.0f, 1.0f, 1.0f };
    	
    	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);    

    	glEnable(GL_LIGHT1);
    }
    else { 
		glDisable(GL_LIGHT1);
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 15.0, -25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glScalef(sizefactor, sizefactor, sizefactor);
    glRotatef(xangle, true, false, false);
    glRotatef(yangle, false, true, false);
    glRotatef(zangle, false, false, true);
    xangle += 2 * int(rotatexaxis);
    yangle += 2 * int(rotateyaxis);
    zangle += 2 * int(rotatezaxis);
    // Draw th emodel
    drawModel();
    // Swap the double buffers
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y) 
{
  	// Keystroke processing here
	rotatexaxis = ((key == 'x') xor (rotatexaxis));
	rotateyaxis = ((key == 'y') xor (rotateyaxis));
	rotatezaxis = ((key == 'z') xor (rotatezaxis));
	sizefactor  = ((1 + (int(key == 'S') * 0.1)) * (1 - (int(key == 's') * 0.1)) * sizefactor);
	light = ((light) xor ((light) and (key == 'l')));
	light = ((light) xor ((!light) and (key == 'L')));
	if (key == 'q') exit(0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jet Display");

    Read(); // Uncomment when the model reader is coded.
    ReadMtl();  // Uncomment when the materials reader is coded
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    // Compute the update rate here...
    glutTimerFunc(1000.0 / updateRate, timer, 0);
    glutMainLoop();
    return 0;
}