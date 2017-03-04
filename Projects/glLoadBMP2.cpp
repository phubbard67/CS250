#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;


// consts
const int WRESX = 900;
const int WRESY = 600;
const int ARESX = 900;
const int ARESY = 600;
int setCont;
int setContDec;
//int setBright;
int sharpCount;


// functions
void display();
void fill(unsigned char arr[ARESY][ARESX][3]);
void print(unsigned char arr[ARESY][ARESX][3]);
void bright(unsigned char arr[ARESY][ARESX][3], int n);
void contrast(unsigned char arr[ARESY][ARESX][3], double n);
void blur(unsigned char arr[ARESY][ARESX][3]);
unsigned char inRange(int n);
unsigned char nbrAvg(unsigned char arr[ARESY][ARESX][3], int r, int c, int color);
void sharpen(unsigned char arr[ARESY][ARESX][3]);
unsigned char nbrSharp(unsigned char arr[ARESY][ARESX][3], int r, int c, int color);



int main(int argc, char** argv) {
    
	cout << "Pick a contrast number: ";
	cin >> setCont;
	//cout << "Pick a brightness number: ";
	//cin >> setBright;
	cout << "Pick a sharpen number: ";
	cin >> sharpCount;
	
	// set up displays and mode (we won't change this!)
	glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    
	// set window size
	glutInitWindowSize (WRESX, WRESY); 
    
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
	static unsigned char arr[ARESY][ARESX][3] = {0};
	fill(arr);
	//bright(arr, setBright);
	//contrast(arr, 0.01);
	contrast(arr, setCont);
	for(int i = 0; i < sharpCount; i++) {
		sharpen(arr);
	}

	// resize the array to the size of the window RES
	glPixelZoom((float)WRESX/ARESX, (float)WRESY/ARESY);
	
	// unpack wrong??
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	// draw the array to the screen
	glDrawPixels(ARESX, ARESY, GL_RGB, GL_UNSIGNED_BYTE, arr);

	// start processing buffered OpenGL routines 
    glFlush ();
}


void blur(unsigned char arr[ARESY][ARESX][3]) {
	unsigned char temp[ARESY][ARESX][3] = {0};
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				temp[r][c][color] = nbrAvg(arr, r, c, color);
			}
		}
	}

	// copy back
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				arr[r][c][color] = temp[r][c][color];
			}
		}
	}
}


unsigned char nbrAvg(unsigned char arr[ARESY][ARESX][3], int r, int c, int color) {
	int avg = 0;
	avg += arr[r-1][c-1][color];
	avg += arr[r-1][c][color];
	avg += arr[r-1][c+1][color];
	avg += arr[r][c-1][color];
	avg += arr[r][c][color];
	avg += arr[r][c+1][color];
	avg += arr[r+1][c-1][color];
	avg += arr[r+1][c][color];
	avg += arr[r+1][c+1][color];
	return avg / 9;
}


void sharpen(unsigned char arr[ARESY][ARESX][3]) {
	unsigned char temp[ARESY][ARESX][3] = {0};
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				temp[r][c][color] = inRange(nbrSharp(arr, r, c, color));
			}
		}
	}
	contrast(temp, 0.1);
	bright(temp, -100);
	// copy back
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				arr[r][c][color] = inRange(arr[r][c][color] + temp[r][c][color]);
			}
		}
	}
}



unsigned char nbrSharp(unsigned char arr[ARESY][ARESX][3], int r, int c, int color) {
	int avg = 0;
	avg += arr[r-1][c-1][color];
	avg += arr[r-1][c][color];
	avg += arr[r-1][c+1][color];
	avg += arr[r][c-1][color];
	avg -= 8*arr[r][c][color];
	avg += arr[r][c+1][color];
	avg += arr[r+1][c-1][color];
	avg += arr[r+1][c][color];
	avg += arr[r+1][c+1][color];
	return avg;
}


void bright(unsigned char arr[ARESY][ARESX][3], int n) {
	for(int r = 0; r < ARESY; r++) {
		for(int c = 0; c < ARESX; c++) {
			for(int color = 0; color < 3; color++) {
				int temp = arr[r][c][color] + n;
				arr[r][c][color] = inRange(temp);
			}
		}
	}
}



void contrast(unsigned char arr[ARESY][ARESX][3], double n) {
	for(int r = 0; r < ARESY; r++) {
		for(int c = 0; c < ARESX; c++) {
			for(int color = 0; color < 3; color++) {
				double temp = arr[r][c][color] - 128;
				temp *= n;
				temp += 128;
				arr[r][c][color] = inRange(temp);
			}
		}
	}
}



unsigned char inRange(int n) {
	if(n > 255) {
		return 255;
	} else if(n < 0) {
		return 0;
	} else {
		return n;
	}
}


void fill(unsigned char arr[ARESY][ARESX][3]) {
	// make file
	ifstream file;
	file.open("Desktop Background.bmp");

	// skip header
	for(int i = 0; i < 54; i++) {
		file.get();
	}

	// puts a color at every array location
	int count = 0;
	for(int r = 0; r < ARESY; r++) {
		for(int c = 0; c < ARESX; c++) {
			arr[r][c][2] = file.get(); // blue
			arr[r][c][1] = file.get(); // green
			arr[r][c][0] = file.get(); // red
			count += 3;
		}

		// skip byte padding
		while(count%4 != 0) {
			file.get();
			count++;
		}
	}

	//print(arr);    // error check
}


void print(unsigned char arr[ARESY][ARESX][3]) {
	for(int r = 0; r < ARESY; r++) {
		for(int c = 0; c < ARESX; c++) {
			cout << (int)arr[r][c][0] << " "
				<< (int)arr[r][c][0] << " "
				<< (int)arr[r][c][0] << "   ";
		}
		cout << endl;
	}
}

