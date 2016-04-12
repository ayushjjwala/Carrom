#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <sys/time.h>
#include <sstream>
#include <iomanip>
#include <string.h>
using namespace std;

#include "striker.cpp"

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

//Function Declarations
void arrangeCoins();
void initRendering();
void handleResize(int w,int h);
void handleKeypress2(int key,int x,int y);
void handleKeypress1(unsigned char key, int x, int y);
void mousehandle(int x,int y);
void movestriker(float speed);
void striker_collideWall();
void strikerCoin_collision();
void coinCoin_collision();
void coin_collideWall();
void movecoin();
int checkRest();
void coinPocket();
void output(float x, float y, float r, float g, float b, const char *string);
void reset();
void update(int value);
void drawBoard();


//Global Variables
float outer_box_len = 6.0f;
float inner_box_len = 5.2f;
float pocket_radius = 0.16f;
float coin_radius = 0.1;
float mouse_x;
float mouse_y;
double t1,t2;
struct timeval tim;
int totalScore = 100;
int check = 0;

//Object initialsation
Board carromBoard;
coins coin[19];
Striker striker;

int main(int argc, char **argv) {

    // Initialize GLUT
    glutInit(&argc, argv);                                  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    gettimeofday(&tim,NULL);
    t1 = tim.tv_sec+(tim.tv_usec/1000000.00);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    // int windowWidth = w * 2 / 3;
    // int windowHeight = h * 2 / 3;

    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("CarromBoard");  // Setup the window
    initRendering();

    arrangeCoins();

    // Register callbacks
    glutDisplayFunc(drawBoard);
    glutIdleFunc(drawBoard);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutReshapeFunc(handleResize);
    glutMotionFunc(mousehandle);

    glutMainLoop();
    return 0;
}

void drawBoard(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -8.0f);

	// Draw Outerbox
    glColor3f(0.54509803921f,0.27058823529f,0.07450980392f);
	carromBoard.drawBox(outer_box_len);

    //Draw Innerbox
	glColor3f(0.94117647058f, 0.90196078431f, 0.54901960784f);
	carromBoard.drawBox(inner_box_len);

    //Draw Design
    carromBoard.drawDesign();    
    glPopMatrix();

    //Draw Pockets
    glColor3f(0.37647058823f,0.37647058823f,0.37647058823f);
    carromBoard.drawPocket(inner_box_len,pocket_radius);

    //Draw Coins
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-8.0f);
    glColor3f(0.0f,0.0f,0.0f);
    for(int i=0;i<=18;i++){
        if(coin[i].absent == 0){
        	if(coin[i].color == 'w' ){
        		glColor3f(1.0f,0.50196078431f,0.0f);
        	}
        	else if(coin[i].color == 'b' ){
        		glColor3f(0.0f,0.0f,0.0f);
        	}
        	else if(coin[i].color == 'r'){
        		glColor3f(0.4f,0.0f,0.0f);
        	}
        	coin[i].drawCoin(coin[i].pos_x,coin[i].pos_y,coin[i].radius);	
        }
    }
    glPopMatrix();

    //Draw Striker
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-8.0f);
    glColor3f(0.0f,0.8,0.0f);
    if(striker.absent == 0){
        striker.drawCoin(striker.pos_x,striker.pos_y,striker.radius);
    }
    glPopMatrix();

    //Draw PowerBar
    glPushMatrix();
    glTranslatef(4.0f,-3.0f,-8.0f);
    glColor3f(0.0f,0.0f,0.0f);
    striker.drawPowerbar();
	striker.applyPower(striker.power);
	glPopMatrix();

	//Draw Aim
	if(striker.no_aim == 0){
		glColor3f(0.0f,0.0f,0.0f);
		striker.takeAim(striker.aim_x,striker.aim_y);
	}

    int time_;
    gettimeofday(&tim,NULL);
    double t2 = tim.tv_sec+(tim.tv_usec/1000000.00);
    time_ = int(t2 - t1);
    totalScore = totalScore - time_;

    // glPushMatrix();
    // glTranslatef(0.0f,0.0f,-8.0f);
    // string text = "Score:";
    // output(-5.0,2.2,0.0,0.0,0.0,text.data());
    // ostringstream convert;
    // convert << totalScore;
    // text = convert.str();
    // output(-5.0,2.0,1.0,0.0,0.0,text.data());
    // glPopMatrix();

    glutSwapBuffers();
}

//Arrange the coins as shown at the start of the game
void arrangeCoins(){
	int i;
	for(i=0;i<18;i++){
		if(i%2 == 0){
			coin[i].color = 'w';
		}
		else{
			coin[i].color = 'b';
		}
		if(i<6){
			coin[i].pos_x = 0.2*cos(DEG2RAD(i*60));
			coin[i].pos_y = 0.2*sin(DEG2RAD(i*60));
		}
		else{
			coin[i].pos_x = 0.4*cos(DEG2RAD((i-6)*30));
			coin[i].pos_y = 0.4*sin(DEG2RAD((i-6)*30));
		}
	}

	coin[i].pos_x = 0.0f;
	coin[i].pos_y = 0.0f;
	coin[i].color = 'r';
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }

    //Move the aim
    if (key == 'w' && (striker.aim_y + coin_radius)< 2.6f && striker.speed == 0){
    	striker.aim_y += 0.05f;
    }
    if (key == 's' && (striker.aim_y - coin_radius)> -2.6f && striker.speed == 0){
    	striker.aim_y -= 0.05f;
    }
    if (key == 'd' && (striker.aim_x + coin_radius)< 2.6f && striker.speed == 0){
    	striker.aim_x += 0.05f;
    }
    if (key == 'a' && (striker.aim_x - coin_radius)> -2.6f && striker.speed == 0){
    	striker.aim_x -= 0.05f;
    }

    //Launch
    if (key == ' ' && striker.power!=0){
        striker.setSpeed(striker.power);
        striker.no_aim = 1;
        striker.power = 0;
        check = 1;
    	glutTimerFunc(10,update,0);
    }
}

void handleKeypress2(int key, int x, int y){
    //move striker
    if(key == GLUT_KEY_LEFT && (striker.pos_x - striker.radius) > -1.7f && striker.speed == 0)
        striker.pos_x -= 0.1;
    if(key == GLUT_KEY_RIGHT && (striker.pos_x + striker.radius) < 1.7f && striker.speed == 0)
        striker.pos_x += 0.1;

    //give striker power
    if(key == GLUT_KEY_UP && striker.power<6.0f && striker.speed == 0)
        striker.power += 0.05f;
    if(key == GLUT_KEY_DOWN && striker.power>0.0f && striker.speed == 0)
        striker.power -= 0.05f;
}

void mousehandle(int x,int y){
    mouse_x = (0.01)*x - 6.86;
    mouse_y = (-0.0089)*y + 3.29;
    if(mouse_x > -1.7f && mouse_x < 1.7f && striker.speed == 0)
        striker.pos_x = mouse_x;
}

void update(int value){
    reset();
    coin_collideWall();
    strikerCoin_collision();
    coinCoin_collision();
    striker_collideWall();
    movecoin();
    movestriker(striker.speed);
    coinPocket();
    striker.strikerPocket(striker.pos_x,striker.pos_y);

    glutTimerFunc(10,update,0);
}

//update the positions of the coins
void movecoin(){
    for(int i=0;i<19;i++){
        coin[i].speed_x = 0.992*coin[i].speed_x;
        coin[i].speed_y = 0.992*coin[i].speed_y;
        if(abs(coin[i].speed_x) < 0.001 && abs(coin[i].speed_y) < 0.001){
            coin[i].speed_x = 0;
            coin[i].speed_y = 0;
        }
        coin[i].pos_x += coin[i].speed_x;
        coin[i].pos_y += coin[i].speed_y;
    }
}

//update the postion of the striker
void movestriker(float speed){
    striker.speed_x = 0.992*striker.speed_x;
    striker.speed_y = 0.992*striker.speed_y;
    if(abs(striker.speed_x) < 0.0001 && abs(striker.speed_y) < 0.0001){
        striker.speed_x = 0;
        striker.speed_y = 0;
    }
	striker.pos_x += striker.speed_x;
	striker.pos_y += striker.speed_y;
}

//check if coins collide the walls of the board n rebound it.
void coin_collideWall(){
    for(int i=0;i<19;i++){
        if(coin[i].pos_x + coin[i].radius > 2.6f && coin[i].speed_x > 0){
            coin[i].speed_x = -1*coin[i].speed_x;
        }

        if(coin[i].pos_x - coin[i].radius < -2.6f && coin[i].speed_x < 0){
            coin[i].speed_x = -1*coin[i].speed_x;
        }

        if(coin[i].pos_y + coin[i].radius > 2.6f && coin[i].speed_y > 0){
            coin[i].speed_y = -1*coin[i].speed_y;
        }

        if(coin[i].pos_y - coin[i].radius < -2.6f && coin[i].speed_y < 0){
            coin[i].speed_y = -1*coin[i].speed_y;
        }
    }
}

//check if striker collides the walls of the board n rebound it.
void striker_collideWall(){
	if(striker.pos_x + striker.radius >= 2.6f){
		striker.speed_x = -1*striker.speed_x;
	}

	if(striker.pos_x - striker.radius < -2.6f){
		striker.speed_x = -1*striker.speed_x;
	}

	if(striker.pos_y + striker.radius >= 2.6f){
		striker.speed_y = -1*striker.speed_y;
	}

	if(striker.pos_y - striker.radius < -2.6f){
		striker.speed_y = -1*striker.speed_y;
	}
}

//check if coin and striker collide and set their velocity accordingly
void strikerCoin_collision(){
	for(int j=0; j < 19 ; j++)
    {
        if(sqrt(pow(striker.pos_x-coin[j].pos_x, 2)+pow(striker.pos_y-coin[j].pos_y, 2)) <= striker.radius + coin[j].radius 
            && coin[j].absent == 0 && striker.absent == 0){

                if((striker.pos_x - coin[j].pos_x)*(striker.speed_x - coin[j].speed_x) +
                 (striker.pos_y - coin[j].pos_y)*(striker.speed_y - coin[j].speed_y) < 0){
                    
                    float z1,z2,q1,q2,q11,q22,z,temp;

                    z1=coin[j].pos_x - striker.pos_x;
                    z2=coin[j].pos_y - striker.pos_y;

                    z=sqrt(z1*z1+z2*z2); // |z|

                    q1=(z1*striker.speed_x + z2*striker.speed_y)/z;
                    q2=(z1*coin[j].speed_x + z2*coin[j].speed_y)/z;
                    temp =q1;
                    q1 = q2;
                    q2 = temp;

                    // Interchange the speed along the line fo collision

                    q11=(z2*striker.speed_x - z1*striker.speed_y)/z;
                    q22=(z2*coin[j].speed_x - z1*coin[j].speed_y)/z;

                    striker.speed_x=((q1*z1+q11*z2)/z);
                    striker.speed_y=((q1*z2-q11*z1)/z);
                    coin[j].speed_x=((q2*z1+q22*z2)/z);
                    coin[j].speed_y=((q2*z2-q22*z1)/z);
                    
                }
        }

    }
}

//check if one coin collides with the other
void coinCoin_collision(){
    for(int i=0 ; i < 19; i++){
        for(int j = i+1; j<19 ; j++){
            if((sqrt(pow(coin[i].pos_x-coin[j].pos_x, 2)+pow(coin[i].pos_y-coin[j].pos_y, 2)) <= coin[i].radius + coin[j].radius) 
                && coin[i].absent ==0 && coin[j].absent ==0 ){

                if((coin[i].pos_x - coin[j].pos_x)*(coin[i].speed_x - coin[j].speed_x) +
                 (coin[i].pos_y - coin[j].pos_y)*(coin[i].speed_y - coin[j].speed_y) < 0){
                    float z1,z2,q1,q2,q11,q22,z;

                    z1=coin[j].pos_x - coin[i].pos_x;
                    z2=coin[j].pos_y - coin[i].pos_y;

                    z=sqrt(z1*z1+z2*z2); // |z|

                    q1=(z1*coin[i].speed_x + z2*coin[i].speed_y)/z;
                    q2=(z1*coin[j].speed_x + z2*coin[j].speed_y)/z;

                    // q1 = projection of v1 along z = v1.z/|z| = component of v1 along the direction of z
                    // q2 = projection of v2 along z = v2.z/|z| = component of v2 along the direction of z

                    float temp;
                    temp =q1;
                    q1 = q2;
                    q2 = temp;

                    // Interchange the velocities along the line fo collision

                    q11=(z2*coin[i].speed_x - z1*coin[i].speed_y)/z;
                    q22=(z2*coin[j].speed_x - z1*coin[j].speed_y)/z;

                    coin[i].speed_x=((q1*z1+q11*z2)/z);
                    coin[i].speed_y=((q1*z2-q11*z1)/z);
                    coin[j].speed_x=((q2*z1+q22*z2)/z);
                    coin[j].speed_y=((q2*z2-q22*z1)/z);

                    coin[i].pos_x += coin[i].speed_x;
                    coin[i].pos_y += coin[i].speed_y;
                    coin[j].pos_x += coin[j].speed_x;
                    coin[j].pos_y += coin[j].speed_y;

                }
            }
        }
    }
}

//check if the coin goes in the pocket
void coinPocket(){
    for(int i=0;i<19;i++){
        if(sqrt(pow(coin[i].pos_x - (2.6 - 0.16),2) + pow(coin[i].pos_y - (2.6 - 0.16),2) < (pocket_radius - coin[i].radius))){
            if(i%2 == 0 && i!=18){
                totalScore += 10;
            }
            else if(i%2 != 0 && i!=18){
                totalScore -= 5;
            }
            else{
                totalScore += 50;
            }
            coin[i].absent = 1;
        }
        else if(sqrt(pow(coin[i].pos_x - (2.6 - 0.16),2) + pow(coin[i].pos_y - (-2.6 + 0.16),2) < (pocket_radius - coin[i].radius))){
            if(i%2 == 0 && i!=18){
                totalScore += 10;
            }
            else if(i%2 != 0 && i!=18){
                totalScore -= 5;
            }
            else{
                totalScore += 50;
            }
            coin[i].absent = 1;
        }
        else if(sqrt(pow(coin[i].pos_x - (-2.6 + 0.16),2) + pow(coin[i].pos_y - (2.6 - 0.16),2) < (pocket_radius - coin[i].radius))){
            if(i%2 == 0 && i!=18){
                totalScore += 10;
            }
            else if(i%2 != 0 && i!=18){
                totalScore -= 5;
            }
            else{
                totalScore += 50;
            }
            coin[i].absent = 1;
        }
        else if(sqrt(pow(coin[i].pos_x - (-2.6 + 0.16),2) + pow(coin[i].pos_y - (-2.6 + 0.16),2) < (pocket_radius - coin[i].radius))){
            if(i%2 == 0 && i!=18){
                totalScore += 10;
            }
            else if(i%2 != 0 && i!=18){
                totalScore -= 5;
            }
            else{
                totalScore += 50;
            }
            coin[i].absent = 1;
        }
        // cout << totalScore << endl;
    }
}

void output(float x, float y, float r, float g, float b, const char *string)
{
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = strlen(string);
    for (i = 0; i < len; i++) {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }   
}
//check if everything comes at rest
int checkRest(){
    int flag = 0;
    for(int i=0;i<19;i++){
        if(striker.speed_x == 0 && striker.speed_y == 0){
            if(coin[i].speed_x == 0 && coin[i].speed_y == 0){
                flag = 1;
            }
            else{
                flag = 0;
                break;
            }
        }
    }
    return flag;
}

//reset the striker if everythin comes at rest
void reset(){
    int rest;
    if(check){
        rest = checkRest();
        if(rest == 1){
            striker.speed = 0;
            striker.pos_x = 0.0f;
            striker.pos_y = -1.85f;
            striker.power = 0;
            striker.no_aim = 0;
            check = 0;
            striker.absent = 0;
        }
    }
}

// Initializing some openGL 3D rendering options
void initRendering(){

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}