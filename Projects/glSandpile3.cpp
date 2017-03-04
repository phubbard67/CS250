#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
using namespace std;


// consts
const int WRES = 796;
const int ARES = 199;


// functions
void display();
void color(unsigned char arr[ARES][ARES][3], int sand[ARES][ARES]);
void fill(int sand[ARES][ARES]);
bool topple(int sand[ARES][ARES]);



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
	glutIdleFunc(glutPostRedisplay);
	
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
	static int sand[ARES][ARES] = {0};
	static bool once = false;
	if(!once) {
		fill(sand);
		once = true;
	}
	topple(sand);
	color(arr, sand);

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


void fill(int sand[ARES][ARES]) {
	bool on = true;
	for(int r = 0; r < ARES; r++) {
		for(int c = 0; c < ARES; c++) {
			if(on) {
				sand[r][c] = 27;
				on = !on;
			} else {
				sand[r][c] = 1;
				on = !on;
			}
		}
	}
}


bool topple(int sand[ARES][ARES]) {
	bool stable = true;
	for(int r = 0; r < ARES; r++) {
		for(int c = 0; c < ARES; c++) {
			// at each cell, check if overfull
			if(sand[r][c] > 3) {
				stable = false;
				// send sand[r][c] / 4 grains to each nbr
				if(r > 0) {
					sand[r-1][c] += sand[r][c] / 4;
				}
				if(r < ARES-1) {
					sand[r+1][c] += sand[r][c] / 4;
				}
				if(c > 0) {
					sand[r][c-1] += sand[r][c] / 4;
				}
				if(c < ARES-1) {
					sand[r][c+1] += sand[r][c] / 4;
				}

				// mod by 4
				sand[r][c] %= 4;
			}
		}
	}
	return stable;
}


void color(unsigned char arr[ARES][ARES][3], int sand[ARES][ARES]) {
	// puts a color at every array location
	for(int r = 0; r < ARES; r++) {
		for(int c = 0; c < ARES; c++) {
			switch(sand[r][c]) {
				case 0:
				arr[r][c][0] = 0; // red
				arr[r][c][1] = 0; // green
				arr[r][c][2] = 0; // blue
				break;
				case 1:
				arr[r][c][0] = 34; // red
				arr[r][c][1] = 139; // green
				arr[r][c][2] = 34; // blue
				break;
				case 2:
				arr[r][c][0] = 255; // red
				arr[r][c][1] = 255; // green
				arr[r][c][2] = 153; // blue
				break;
				case 3:
				arr[r][c][0] = 112; // red
				arr[r][c][1] = 128; // green
				arr[r][c][2] = 144; // blue
				break;
				default:
				arr[r][c][0] = 255; // red
				arr[r][c][1] = 255; // green
				arr[r][c][2] = 255; // blue
			}
		}
	}
}




