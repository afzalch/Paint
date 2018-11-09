#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "mathlib.h"
#include <vector>
#include <math.h>

// Issues I was unable to fix
// 1 - Implementing bresehnam algorithm properly
// 2 - Colours of all shapes other than points were not separate, so if I drew one circle that is red and after that
// 	   a green circle, then the initially red circle would also become green
// 3 - Didn't implement radial brush 


#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
using namespace std;

//Globals

const float PI = 3.1415927;
point2D p;
std::vector<float> points; //for scenarios where the shape is points?
std::vector<point2D> lines; //used to ensure that their are at least 2 points in lines vector before circle is drawn
std::vector<point2D> rectangles; //used to ensure that their are at least 2 points in rectangles vector before circle is drawn
std::vector<point2D> circles; //used to ensure that their are at least 2 points in circle vector before circle is drawn
std::vector<colour> colours;


// Make an enums to represent the current shape
// enum colour {red, green, blue, purple, yellow};
enum shapes {Point, Line, Rectangle, Circle};

colour c = {255,0,0};
shapes s = Point;

/*RGB values for colours
	red - (255,0,0)
	green - (0,255,0)
	blue - (0,0,255)
	purple - (102,0,255)
	yellow - (255,255,102)
*/


// Creating a keyboard input that whenever the escape key or q is pressed the window closes
void keyboard(unsigned char key, int xIn, int yIn){
	int mod = glutGetModifiers();
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
	}
}


// Draw all points in the points vector
void point(){
	glBegin(GL_POINTS);
    	for (int i=0; i < points.size(); i += 2){
			glColor3f(colours[i/2].r,colours[i/2].g,colours[i/2].b);
        	glVertex2i(points[i], points[i+1]);
    	}
    glEnd();
}


// Calculating points needed to draw circle
void circle(int x, int y, float r){
	for (float angle = 0; angle <= 2*PI; angle+=0.01){
        float X =(r) * cos (angle);
        float Y =(r) * sin (angle);
		point2D p1 = {X+x,Y+y};
        points.push_back(X+x);
		points.push_back(Y+y);
		colour currentC = {c.r/255,c.g/255,c.b/255};
		colours.push_back(currentC);
		point();
    }
}

// bresenhams algorithm
void bres(int x1,int y1, int x2, int y2){     
    int dx = x2 - x1;
	int dy = y2 - y1;
    int d = 2*dy-dx;
	int incrE = dy*2;
	int incrNE = 2*dy-2*dx;
	float Slope;
    int S, O, End;
	Slope = (float) dy/dx;


	if(dx == 0){
		if(y1<y2){
			S=y1;
			while(S<y2){
				S++;
				points.push_back(x1);
				points.push_back(S);
				colour currentC = {c.r/255,c.g/255,c.b/255};
				colours.push_back(currentC);
				point();
			}
		} else{
			S = y2;
			while(S<y1){
				S++;
				points.push_back(x1);
				points.push_back(S);
				colour currentC = {c.r/255,c.g/255,c.b/255};
				colours.push_back(currentC);
				point();
			}
		}
		
	} else {
		if (Slope >-1 && Slope <1){
			if(x1>x2){
        		S=x2;
        		O=y2;
        		End=x1;
			} else{
				S=x1;
				O=y1;
				End=x2;
			}
		} else {
			incrE = 2*dx;
			incrNE = 2*dx - 2*dy;
			d = 2*dx - dy;
			if(y1>y2){
				O = x2;
				S = y2;
				End = y1;
			} else {
				O = x1;
				S = y1;
				End = y2;
			}
		}
		while(S<=End){
			S++;
			if(d<0){
				d += incrE;
			}
			else{
				d += incrNE;
				if(Slope>0.0){
					O++;
				}else {
					O--;
				}
		}
		points.push_back(S);
		points.push_back(O);
		colour currentC = {c.r/255,c.g/255,c.b/255};
		colours.push_back(currentC);
		point();
		}
	}
}

// determining points and drawing rectangle
void rectangle(int x1, int y1, int x2, int y2){
	int x3,x4,y3,y4;
	// x1 - point in top left corner of rectangle
	// x2 - point in bottom right corner of rectangle
	// x3 - point in top right corner of rectangle
	// x4 - point in bottom left corner of rectangle
	x3 = x2; 
	x4 = x1;
	y3 = y1;
	y4 = y2;
	if(x3 == x2 && x4 == x1){
		if (y3 == y1 && y4 == y2){
			bres(x1,y1,x3,y3);
			bres(x1,y1,x4,y4);
			bres(x3,y3,x2,y2);
			bres(x4,y4,x2,y2);
		} else {
			printf("Error as points arent equivalent values\n");
		}
	}
	glutPostRedisplay();
	
}

// Used to allow for freeform click and drag functionality for points shape
void motion(int x, int y){
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	float actualY = h-y;
	points.push_back(x);
	points.push_back(actualY);
	colour currentC = {c.r/255,c.g/255,c.b/255};
	colours.push_back(currentC);
	glutPostRedisplay();
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    glPointSize(5.0);
	point();
	//switch our buffers for a smooth animation
	glutSwapBuffers();
}


void mouse(int btn, int state, int x, int y){
	// Size of window is 400,400 according to main function
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		
		// calculating actual points coordinate
		float actualY = 400-y;
		p.x = x;
		p.y = actualY;
		colour currentC = {c.r/255,c.g/255,c.b/255};
		
		
		if (s == Point){ // && state == GLUT_DOWN
			points.push_back(x);
			points.push_back(actualY);
			colours.push_back(currentC);
			glutMotionFunc(motion);
		} else if (s == Line ){ // && state == GLUT_DOWN
			lines.push_back(p);
			colours.push_back(currentC);
			// Only calling bres function when user has clicked the correct amount of times on the window
			if(lines.size() >= 2 && lines.size() % 2 == 0){
				for (int i = 0; i < lines.size(); i+=2){
					bres(lines[i].x,lines[i].y,lines[i+1].x,lines[i+1].y);
				}
			} 
			
		} else if (s == Rectangle){ // && state == GLUT_DOWN
			rectangles.push_back(p);
			colours.push_back(currentC);
			// Only calling rectange function when user has clicked the correct amount of times on the window
			if(rectangles.size() >= 2 && rectangles.size() % 2 == 0){
				for (int i = 0; i < rectangles.size(); i+=2){
					rectangle(rectangles[i].x,rectangles[i].y,rectangles[i+1].x,rectangles[i+1].y);
				}
			} else {
				printf("Waiting for another point before rectangle can be drawn!\n");
			}
		} else if (s == Circle){
			circles.push_back(p);
			// Only calling circle function when user has clicked the correct amount of times on the window
			if(circles.size() >= 2 && circles.size() % 2 == 0){
				for (int i = 0; i < circles.size(); i+=2){
					// calculating radius of circle
					int distanceX = circles[i].x - circles[i+1].x;
					int distanceY = circles[i].y - circles[i+1].y;
					int distanceSq = pow(distanceX,2) + pow(distanceY,2);
					float distance = pow(distanceSq,0.5);

					// calling circle func
					circle(circles[i].x,circles[i].y,distance);
				}
			} else {
				printf("Waiting for another point before circle can be drawn!\n");
			}
		}
		glutPostRedisplay();
	}
}





/* Function's purpose is to ensure that the objects retain the same porportions 
regardless of what size the actual window will be
   */
void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, w, 0, h);
	gluPerspective(45, (float)((w+0.0f)/h), 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, 400, 400);
}



void callBackInit(){
	glutDisplayFunc(display);	//registers "display" as the display callback function
	gluOrtho2D(-25,25,-25,25);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
}


// Setting up what occurs when an item from menu has been selected
void menuProc(int value){
// int r = 255;
// int g = 0;
// int b = 0;

/*RGB values for colours
	red - (255,0,0)
	green - (0,255,0)
	blue - (0,0,255)
	purple - (102,0,255)
	yellow - (255,255,102)
*/

// values: 1-5 are to change colour and are done by changing values of global colour variable
// shapes: 6-9 are to change shapes are are done by changing values of shape enum
	if (value == 1) {
		printf("Colour:Red\n");
		c.r = 255;
		c.g = 0;
		c.b = 0;
	} else if (value == 2) {
		printf("Colour:Green\n");
		c.r = 0;
		c.g = 255;
		c.b = 0;
		
	} else if (value == 3) {
		printf("Colour:Blue\n");
		c.b = 255;
		c.g = 0;
		c.r = 0;
	} else if (value == 4) {
		printf("Colour:Purple\n");
		c.r = 102;
		c.g = 0;
		c.b = 255;
	} else if (value == 5) {
		printf("Colour:Yellow\n");
		c.r = 255;
		c.g = 255;
		c.b = 102;
	} else if (value == 6) {
		printf("Shape:Point\n");
		s = Point;
	} else if (value == 7) {
		printf("Shape:Line\n");
		s = Line;
	} else if (value == 8) {
		printf("Shape:Rectangle\n");
		s = Rectangle;
	} else if (value == 9) {
		printf("Shape:Circle\n");
		s = Circle;
	} else if(value == 10){
		points.clear();
		circles.clear();
		lines.clear();
		rectangles.clear();
		s = Point;
		glutPostRedisplay();
		printf("Colour: Red\n Shape: Point");
	} else if (value == 11) {
		printf("Program was quit!\n");
		exit(0);
	}
}



// Creating menu which works by clicking the right click button
void createOurMenu(){
	//int subMenu_id = glutCreateMenu(menuProc2);
    // Submenus for the colour and shape options
	int subMenu_id = glutCreateMenu(menuProc);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);

	int subMenu_id2 = glutCreateMenu(menuProc);
	glutAddMenuEntry("Point", 6);
	glutAddMenuEntry("Line", 7);
	glutAddMenuEntry("Rectangle", 8);
	glutAddMenuEntry("Circle", 9);

    // Creating the main entries of the menu
	int main_id = glutCreateMenu(menuProc);
	glutAddSubMenu("Colour", subMenu_id);
	glutAddSubMenu("Shapes", subMenu_id2);
	glutAddMenuEntry("Clear", 10);
	glutAddMenuEntry("Quit", 11);
	

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* main function - program entry point */
int main(int argc, char** argv)
{

	// Describing/listing all the keyboard/mouse/menu commands and how they are used in the program 
	// to the command prompt
	cout << "-----------------------------------ReadMe------------------------------------------\n";
	cout << "First, please resize the window as nothing appears on Mac devices until window was\n "
			"resized a bit. Then, clicking the left mouse button on the window will cause points to \n"
			"appear. To change the shapes that are created by a mouse click, right click and look at  \n"
			"the menu options. The menu which has the following menu items: \n"
			"\"Colour\" - Has submenu with all different colours options\n"
			"\t\"Red\" - Default colour\n"
			"\t\"Green\" - Colour option\n"
			"\t\"Blue\" - Colour option\n"
			"\t\"Purple\" - Colour option\n"
			"\t\"Yellow\" - Colour option\n"
			"\"Shapes\" - Has submenu with all different shape options\n"
			"\t\"Point\" - Default shape\n"
			"\t\"Line\" - Shape option\n"
			"\t\"Rectangle\" - Shape option\n"
			"\t\"Circle\" - Shape option\n"
			"\"Clear\" - Clear the window \n"
			"\"Quit\" - Quit the program.\n"
			"Furthermore, there is one keyboard command which is that whether the letter \"q\" or the\n"
			"Escape character is presed, the program will be quit!\n";


	cout << "-----------------------------------------------------------------------------------\n";


	glutInit(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3GC3 Lecture Code 1");	//creates the window
	callBackInit();
	// init();
	createOurMenu();
	// gluOrtho2D(0,400,0,400);
	glOrtho(0.0f, 400, 400, 0.0f, 0.0f, 1.0f);
	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}









