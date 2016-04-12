#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

#include "board.cpp"

class coins: public Board
{
	public:
		float radius;
		float coeff_friction;
		char color;
		float speed_x;
		float speed_y;
		float pos_x;
		float pos_y;
		float mass;
		int absent;

		coins(){
			 radius = 0.1f;
			 coeff_friction = 0.8f;
			 mass = 5.0f;  
			 absent = 0;
		}
		void drawCoin(float x,float y,float radius){
			glPushMatrix();
			glTranslatef(x,y,0.0f);
			drawBall(radius);
			glPopMatrix();
		}
};