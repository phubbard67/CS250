#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
using namespace std;


// consts
const int WRES = 512;
const int ARES = 64;
const int VARS = log2(ARES);


// functions
void display();
void fill(unsigned char arr[ARES][ARES][3]);
bool expression(bool vars[VARS], unsigned char arr[ARES][ARES][3], int r, int c);
bool imp(bool, bool);
bool bii(bool, bool);



int main(int argc, char** argv) {
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
	// puts a color at every array location
	for(int r = 0; r < ARES; r++) {
		for(int c = 0; c < ARES; c++) {
			// this holds logical variables
			bool vars[VARS] = {0};

			// populate the array with truth values
			int temp = r;
			for(int i = VARS-1; i >= 0; i--) {
				// convert current r value into binary
				vars[i] = temp % 2;
				temp /= 2;
			}
			
			// this sets truth value for a table entry
			bool truth = expression(vars, arr, r, c);
			
			// this draws truth values
			arr[r][c][0] = 255 * truth;
			arr[r][c][1] = 255 * truth;
			arr[r][c][2] = 255 * truth;
		}
	}
}


bool expression(bool vars[VARS], unsigned char arr[ARES][ARES][3], int r, int c) {
	/*switch(c) {
		case 0:
			return vars[0] && vars[1];
		case 1:
			return !vars[1];
		case 2:
			return !(vars[0] && vars[1]);
		case 3:
			return !(vars[0] || vars[1]);
	}*/

	// recursive scalable version
	// base cases
	if(c < VARS) {
		return vars[c];
	}

	// recursive cases
	return  bii(arr[r][c-2][0], arr[r][c][0]), 
			arr[r][c][0], 
			(!arr[r][c-1][0] || arr[r][c-3][0]), 
			(arr[r][c-2][0] && !arr[r][c-5][0]), 
			arr[r][c-4][0] && arr[r][c-6][0];
	
}

bool imp(bool P, bool Q) {
	return !P || Q;
}

bool bii(bool P, bool Q) {
	return imp(P, Q) && imp(Q, P);
}



