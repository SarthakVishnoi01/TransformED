#ifndef COMMON_H
#define COMMON_H

//#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <GL/freeglut.h>
//#include "freeglut.h"
#include <iostream> 
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <gtk/gtk.h>
using namespace std;



void initGL();
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void reshape(GLsizei width, GLsizei height);
void edge_transformations();
void get_coordinates();
void face_detection();
bool is_Equal_Zero(float a);
void call_3D_to_2D(int argc, char** argv);
void call_2D_view(int argc, char** argv);
void call_3D_view(int argc, char** argv);
void call_2D_t0_3D(int argc, char** argv);


#endif
