#include <iostream>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <ctime> // Needed for the true randomization


using namespace std;
#define DEF_floorGridScale	1.0f
#define DEF_floorGridXSteps	15.0f
#define DEF_floorGridZSteps	15.0f
#define line_size 2.0f
#define number_bricks 35
#define number_special 5
#define number_bonus 6
#define brick_width 3.0f
#define brick_height 1.0f
#define platform_width 4.0f
#define platform_height 0.5f
#define platform_y_position -12.0f
float platform_x_position = 0.0f;



void init(){

}

void ejesCoordenada(float w) {
	
	glLineWidth(w);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(w-1.0);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}



void drawPoint (int x, int y, int size, float r,float g, float b){ 
	glPointSize(size);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2){
	glLineWidth(line_size);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
	glEnd();
}

void drawCircle(int x, int y, int radius, int size, float r,float g, float b){
	int i;
	int lineAmount = 500;
	GLfloat twicePi = 2.0f * 3.141592;
	
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) { 
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}


void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20;
	GLfloat twicePi = 2.0f * 3.141592;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}






void keyPressed (unsigned char key, int x, int y) {  
	switch (key)
	{
		case 'a': 
		case 'A':
			if(platform_x_position > -16.0f){platform_x_position -= 1.5f;}
			break; 
		case 'd':
		case 'D':
			if(platform_x_position < 16.0f){platform_x_position += 1.5f;}
			break;
		default:
			break;
	};
	
}  

void changeViewport(int w, int h) {
	glViewport(0,0,w,h);
	float aspectratio;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	aspectratio = (float) w / (float) h;
	if (w<=h)
		glOrtho(-10,10,10/aspectratio,-10/aspectratio,1.0,-1.0);
	else
		glOrtho(-10*aspectratio,10*aspectratio,10,-10,1.0,-1.0);
}

void renderGrid(){
	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glPushMatrix();
    glColor3f( 0.0f, 0.7f, 0.7f );
	glLineWidth(3);
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0f );
	glVertex3f( xLocal, zExtent,  0.0f );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0f );
	glVertex3f(  xExtent, zLocal, 0.0f );
	}
    glEnd();
    glPopMatrix();
}

class Platform {
	public:
		float x_position;
		float y_position;
		float width;
		float height;
		float color [3];

	Platform(float x, float y, float w, float h){
		x_position = x;
		y_position = y;
		width = w;
		height = h;
	};

	void drawPlatform(){
		glPushMatrix();
			glColor3f(0,0,255);
			drawLine(x_position+width/2,y_position+height/2,x_position-width/2,y_position+height/2);
			drawLine(x_position-width/2,y_position+height/2,x_position-width/2,y_position-height/2);
			drawLine(x_position+width/2,y_position-height/2,x_position-width/2,y_position-height/2);
			drawLine(x_position+width/2,y_position-height/2,x_position+width/2,y_position+height/2);
		glPopMatrix();
	};

	void setPlatformXposition(float x){
		//direction is 1 or -1
		x_position = x;
	};
};



class Brick{
	public:
		int id;
		float x_position;
		float y_position;
		float width;
		float height;
		int bonus;
		bool special;
		int hits;
		float color [3];
	
	Brick(int id_num,float x,float y,float w, float h, int bonus_type,bool is_special){
		id = id_num;
		x_position = x;
		y_position = y;
		width = w;
		height = h;
		bonus = bonus_type;
		special = is_special;
		if (special){
			hits = 2;
			color[0] = 1;
			color[1] = 1;
			color[2] = 1;
		}
		else{
			hits = 1;
			color[0] = 1;
			color[1] = 0;
			color[2] = 0;
		};
	};

	void drawBrick(){
		glPushMatrix();
			glColor3f(color[0],color[1],color[2]);
			drawLine(x_position,y_position,x_position,y_position+height);
			drawLine(x_position,y_position+height,x_position+width,y_position+height);
			drawLine(x_position+width,y_position+height,x_position+width,y_position);
			drawLine(x_position+width,y_position,x_position,y_position);
		glPopMatrix();
	};

	void print(){
		printf("id: %d, x_position: %f, y_position: %f, width: %f, height: %f, bonus: %d \n",id,x_position,y_position,width,height,bonus);
	};
};

class Manager{
	public:
		std::vector<Brick>  level_bricks;
		float brick_separation;
		float initial_x;
		float initial_y;

	Manager(float brick_gap,float start_x, float start_y){
		brick_separation = brick_gap;
		initial_x = start_x;
		initial_y = start_y;
		bool random_special;
		bool has_bonus;
		int random_bonus;
		int probability_bonus = 3; // start with 15% probability of containing a bonus;
		int probability_special = 3; // start with 15% probability of becoming a special brick;
		int countr_special = 0;
		int countr_bonus = 0;
		float acc_x = initial_x;
		float acc_y = initial_y;
		srand( time(0));

		for (int i = 0; i<number_bricks;i++){
			if ((i % 7 == 0) && i != 0){
				acc_y -= (brick_height + brick_gap);
				acc_x = start_x;
			}
			if (countr_special < number_special ) random_special = ((rand() % 20) <= probability_special);
			else if (countr_special = number_special) random_special = false;
			if (random_special) countr_special++;
			if (i > 20 && countr_special <= 3) probability_special += 2; // increase to 25% probability if not enought have been draw;

			if (countr_bonus < number_bonus ) has_bonus = ((rand() % 20) <= probability_bonus); 
			else if (countr_bonus = number_bonus) has_bonus = 0;
			if (has_bonus) countr_bonus++;
			if (i > 20 && countr_bonus <= 3) probability_bonus += 2; // increase to 25% probability if not enought have been draw;
			if (has_bonus) random_bonus = (rand() % 2) + 1;
			else random_bonus = 0;

			Brick brick(i,acc_x,acc_y,brick_width,brick_height,random_bonus,random_special);
			level_bricks.push_back(brick);
			acc_x += brick_width + brick_separation;
		}

	};

	void drawBricks(){
		for (std::vector<Brick>::iterator brick = level_bricks.begin() ; brick != level_bricks.end(); ++brick){
			(*brick).drawBrick();
			//(*brick).print();
		}
		
	}
};

void render(){
	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(140.0, w/h, 3.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0,5.0,
			  0.0, 0.0, 0.0,
              0.0,1.0, 0.0);
	//renderGrid();
	drawPoint(0,0,50,1,1,1);
	Manager test(1,-13,11);
	Platform platform(0.0,platform_y_position,platform_width,platform_height);
	platform.setPlatformXposition(platform_x_position);
	platform.drawPlatform();
	test.drawBricks();

	glutSwapBuffers();
	//11x 11y
}



int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(800,600);

	glutCreateWindow("Opengl");
	init();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutIdleFunc(render);
	glutKeyboardFunc(keyPressed);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 0;

}