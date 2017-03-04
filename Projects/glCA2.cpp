#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
using namespace std;


// consts
const int WRES = 720;
const int ARES = 324;
int RULE;


// functions
void display();
void fill(unsigned char arr[ARES][ARES][3]);
bool apply(unsigned char arr[ARES][ARES][3], int r, int c);



int main(int argc, char** argv) {
	int number;
	cout << "Enter a number between 0 and 256: ";
	cin >> number;

	RULE = number;
	

    // set up displays and mode (we won't change this!)
	glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    
	// set window size
	glutInitWindowSize (WRES, WRES); 
    
	// set window position on screen
	glutInitWindowPosition (0, 0);

	// set window title
    glutCreateWindow ("test");
    
    // set the display function to the one we made below
	glutDisplayFunc(display);
	
	// required to make the thing go 
    glutMainLoop();

	// exit
    return 0;
}


void display() {
	//  clear all pixels
    glClear (GL_COLOR_BUFFER_BIT);

	// make and fill in array
	static unsigned char arr[ARES][ARES][3] = {0};
	fill(arr);

	// resize the array to the size of the window RES
	glPixelZoom((float)WRES/ARES, (float)WRES/ARES);
	
	// unpack wrong??
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	// draw the array to the screen
	glDrawPixels(ARES, ARES, GL_RGB, GL_UNSIGNED_BYTE, arr);

	// start processing buffered OpenGL routines 
    glFlush ();
}


void fill(unsigned char arr[ARES][ARES][3]) {
	// base case (top middle pixel is true)
	arr[ARES-1][ARES/2][0] = 255;
	arr[ARES-1][ARES/2][1] = 255;
	arr[ARES-1][ARES/2][2] = 255;

	// puts a color at every array location
	for(int r = ARES-2; r >= 0; r--) {
		for(int c = 1; c < ARES-1; c++) {
			// this sets truth from applying our rule
			bool truth = apply(arr, r+1, c);
			
			// this draws truth values
			arr[r][c][0] = 156 * truth;
			arr[r][c][1] = 112 * truth;
			arr[r][c][2] = 25 * truth;
		}
	}
}


bool apply(unsigned char arr[ARES][ARES][3], int r, int c) 
{
	bool A = arr[r][c-1][0];
	bool B = arr[r][c][0];
	bool C = arr[r][c+1][0];
	bool result = false;

	
	// rule 30
	//result = (A&&!B&&!C) || (!A&&B&&C) || (!A&&B&&!C) || (!A&&!B&&C);
	
	// cycle through all 8 possibilities of 3 sq coloring
	for(int i = 7; i >= 0; i--) {
		bool clause = true;
		if((1<<i) & RULE) {
			clause &= (i&4) ? A : !A;
			clause &= (i&2) ? B : !B;
			clause &= (i&1) ? C : !C;
			result |= clause;
		}
	}

	return result;
}

