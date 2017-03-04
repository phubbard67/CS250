#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <gmpxx.h>
#include <string>
using namespace std;


// consts
const int WRESX = 900;
const int WRESY = 900;
const int ARESX = 900;
const int ARESY = 900;


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
	//bright(arr, -15);
	//contrast(arr, 10);
	//contrast(arr, 100);
	for(int i = 0; i < 5; i++) {
		blur(arr);
	}
	contrast(arr, 10);

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
	double avg = 0;
	avg += arr[r-1][c-1][color];
	avg += arr[r-1][c][color];
	avg += arr[r-1][c+1][color];
	avg += arr[r][c-1][color];
	avg += arr[r][c][color];
	avg += arr[r][c+1][color];
	avg += arr[r+1][c-1][color];
	avg += arr[r+1][c][color];
	avg += arr[r+1][c+1][color];
	return round(avg / 9);
}


void sharpen(unsigned char arr[ARESY][ARESX][3]) {
	unsigned char temp[ARESY][ARESX][3] = {0};
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				temp[r][c][color] = nbrSharp(arr, r, c, color);
			}
		}
	}
	//contrast(temp, 0.2);
	//bright(temp, -150);
	// copy back
	for(int r = 1; r < ARESY-1; r++) {
		for(int c = 1; c < ARESX-1; c++) {
			for(int color = 0; color < 3; color++) {
				arr[r][c][color] = inRange( temp[r][c][color]);
			}
		}
	}
}



unsigned char nbrSharp(unsigned char arr[ARESY][ARESX][3], int r, int c, int color) {
	int avg = 0;
	avg -= arr[r-1][c-1][color];
	avg -= arr[r-1][c][color];
	avg -= arr[r-1][c+1][color];
	avg -= arr[r][c-1][color];
	avg += 8*arr[r][c][color];
	avg -= arr[r][c+1][color];
	avg -= arr[r+1][c-1][color];
	avg -= arr[r+1][c][color];
	avg -= arr[r+1][c+1][color];
	return inRange(avg);
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
	// how big is my decimal
	const int NUMBITS = 24*ARESX*ARESY;
	mpf_set_default_prec(NUMBITS*4);

	// get the num and den from the user
	string n, d;
	cout << "Enter the numerator: ";
	cin >> n;
	cout << endl << "Enter the denominator: ";
	cin >> d;
	
	// convert strings into mpf
	mpf_class num(n);
	mpf_class den(d);
	mpf_class quotient = num / den;

	// transform quotient back into string
	mp_exp_t exp;
	string str = quotient.get_str(exp, 2, NUMBITS);
	
	// reinsert leading zeroes
	for(int i = 0; i > exp; i--) {
		str.insert(0, "0");
	}

	// print our number
	//cout << "The string is: " << str << endl;
	//cin >> n;

	// convert number to byte sized chunks
	string byt;
	for(int i = 0; 8*i < NUMBITS; i++) {
		byt = str.substr(8*i, 8);
		arr[(i/3)/ARESX][(i/3)%ARESX][i%3] = strtol(byt.c_str(), NULL, 2);
	}

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


