#include <iostream>
#include <cmath>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

#include "coins.cpp"

class Striker: public coins
{	
	public:
		float power;
		float aim_x;
		float aim_y;
		float speed;
		float speed_x;
		float speed_y;
		int no_aim;
		Striker(){
			radius = 0.15f;
			mass = 10.0f;
			pos_x = 0.0f;
			pos_y = -1.85f;
			power = 0.0f;
			aim_x = 0.0f;
			aim_y = 0.0f;
			no_aim = 0;
			speed = 0;
			absent = 0;
		}

		void drawPowerbar(){
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		    glBegin(GL_QUADS);
		    glVertex2f(-0.25f, 0.0f);
		    glVertex2f(0.25f, 0.0f);
		    glVertex2f(0.25f, 6.0f);
			glVertex2f(-0.25f, 6.0f);
		    glEnd();
		    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		void applyPower(float y){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);
			glVertex2f(-0.23f, 0.0f);
			glVertex2f(0.23f, 0.0f);
			glVertex2f(0.23f, y);
			glVertex2f(-0.23f, y);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		void takeAim(float x,float y){
			glPushMatrix();
			glTranslatef(x,y,-8.0f);
			drawCircle(0.05f);
			glPopMatrix();
		}

		void setSpeed(float y){
			float temp;
			speed = y*0.05;
			temp = (aim_y - pos_y)*(aim_y - pos_y)/((aim_x - pos_x)*(aim_x - pos_x));
			speed_x = speed*(sqrt(1/(temp+1)));
			if(aim_x < pos_x)
				speed_x = -1*speed*(sqrt(1/(temp+1)));				
			speed_y = speed*(sqrt(temp/(temp+1)));
			if(aim_x - pos_x == 0){
				speed_x = 0;
				speed_y = speed;
			}
		}
		
		void strikerPocket(float x,float y){
		    if(sqrt(pow(x - (2.6 - 0.16),2) + pow(y - (2.6 - 0.16),2) < (0.16 - radius))){
		        absent = 1;
		        speed_x = 0;
		        speed_y = 0;
		    }
		    if(sqrt(pow(x - (2.6 - 0.16),2) + pow(y - (-2.6 + 0.16),2) < (0.16 - radius))){
		        absent = 1;
		        speed_x = 0;
		        speed_y = 0;

		    }
		    if(sqrt(pow(x - (-2.6 + 0.16),2) + pow(y - (2.6 - 0.16),2) < (0.16 - radius))){
		        absent = 1;
		        speed_x = 0;
		        speed_y = 0;  
		    }
		    if(sqrt(pow(x - (-2.6 + 0.16),2) + pow(y - (-2.6 + 0.16),2) < (0.16 - radius))){
		        absent = 1;
		        speed_x = 0;
		        speed_y = 0;
		    }
		}
};