#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

class Board{
	public:
		void drawBox(float len) {
		   
		    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		    glBegin(GL_QUADS);
		    glVertex2f(-len / 2, -len / 2);
		    glVertex2f(len / 2, -len / 2);
		    glVertex2f(len / 2, len / 2);
			glVertex2f(-len / 2, len / 2);
		    glEnd();
		    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
		}

		void drawBall(float rad){
		   glBegin(GL_TRIANGLE_FAN);
		   for(int i=0 ; i<360 ; i++) {
		       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
		   }
		   glEnd();
		}

		void drawPocket(float inner_box_len, float pocket_radius) {
		    glPushMatrix();
		    glTranslatef(-inner_box_len/2 + pocket_radius, -inner_box_len/2 + pocket_radius, -8.0f);    
		    drawBall(pocket_radius);
		    glPopMatrix();

		    glPushMatrix();
		    glTranslatef(-inner_box_len/2 + pocket_radius, inner_box_len/2 - pocket_radius, -8.0f);    
		    drawBall(pocket_radius);
		    glPopMatrix();

		    glPushMatrix();
		    glTranslatef(inner_box_len/2 - pocket_radius, inner_box_len/2 - pocket_radius, -8.0f);    
		    drawBall(pocket_radius);
		    glPopMatrix();

		    glPushMatrix();
		    glTranslatef(inner_box_len/2 - pocket_radius, -inner_box_len/2 + pocket_radius, -8.0f);    
		    drawBall(pocket_radius);
		    glPopMatrix();
		}

		void drawCircle(float r){
		    static const double inc = M_PI / 12;
		    static const double max = 2 * M_PI;
		    glBegin(GL_LINE_LOOP);
		    for(double d = 0; d < max; d += inc) {
		        glVertex2f(cos(d) * r, sin(d) * r);
		    }
		    glEnd();
		}

		void drawDesign(){		   
		    glColor3f(0.0f,0.0f,0.0f);
		    drawBox(4.0f);
		    glColor3f(0.94117647058f, 0.90196078431f, 0.54901960784f);
		    drawBox(3.963f);
		    glColor3f(0.0f,0.0f,0.0f);
		    drawBox(3.42f);    
		    glColor3f(0.94117647058f, 0.90196078431f, 0.54901960784f);
		    drawBox(3.4f);
		    glColor3f(0.0f,0.0f,0.0f);
		    drawBall(0.52f);
		    glColor3f(0.94117647058f, 0.90196078431f, 0.54901960784f);
		    drawBall(0.5f);
		    glColor3f(0.0f,0.0f,0.0f);
		    drawCircle(0.3f);
		    glColor3f(1.0f,0.4f,0.4f);
		    drawBall(0.1f);
		}

};