//
//  Window.cpp
//  CSE167 Final Project
//
//  Created by Ruiqing Qiu on 12/7/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Window.h"
#include <stdlib.h>

#include <math.h>
#define PI 3.14159265f

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
Matrix4 Window::world = Matrix4();

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    cout << w << ", " << h << endl;
    cerr << "Window::reshapeCallback called" << endl;
    width = w;
    height = h;
    glViewport(0, 0, w, h);  // set new viewport size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0);// set perspective projection viewing frustum
    glTranslatef(0, 0, -20);    // move camera back 20 units so that it looks at the origin (or else it's in the origin)
    glMatrixMode(GL_MODELVIEW);
}
void Window::displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
    // Tell OpenGL what ModelView matrix to use:
    Matrix4 glmatrix;
    glmatrix = Globals::cube.getMatrix();
    glmatrix.transpose();
    glLoadMatrixd(glmatrix.getPointer());
    // Draw all six faces of the cube:
    glBegin(GL_QUADS);
    glColor3f(0.0, 1.0, 0.0);		// This makes the cube green; the parameters are for red, green and blue.
        // To change the color of the other faces you will need to repeat this call before each face is drawn.
        // Draw front face:
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(-5.0,  5.0,  5.0);
        glVertex3f( 5.0,  5.0,  5.0);
        glVertex3f( 5.0, -5.0,  5.0);
        glVertex3f(-5.0, -5.0,  5.0);
        
        // Draw left side:
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(-5.0,  5.0,  5.0);
        glVertex3f(-5.0,  5.0, -5.0);
        glVertex3f(-5.0, -5.0, -5.0);
        glVertex3f(-5.0, -5.0,  5.0);
        
        // Draw right side:
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f( 5.0,  5.0,  5.0);
        glVertex3f( 5.0,  5.0, -5.0);
        glVertex3f( 5.0, -5.0, -5.0);
        glVertex3f( 5.0, -5.0,  5.0);
        
        // Draw back face:
        glNormal3f(0.0, 0.0, -1.0);
        glVertex3f(-5.0,  5.0, -5.0);
        glVertex3f( 5.0,  5.0, -5.0);
        glVertex3f( 5.0, -5.0, -5.0);
        glVertex3f(-5.0, -5.0, -5.0);
        
        // Draw top side:
        glNormal3f(0.0, 1.0, 0.0);
        glVertex3f(-5.0,  5.0,  5.0);
        glVertex3f( 5.0,  5.0,  5.0);
        glVertex3f( 5.0,  5.0, -5.0);
        glVertex3f(-5.0,  5.0, -5.0);
        
        // Draw bottom side:
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f(-5.0, -5.0, -5.0);
        glVertex3f( 5.0, -5.0, -5.0);
        glVertex3f( 5.0, -5.0,  5.0);
        glVertex3f(-5.0, -5.0,  5.0);
    
        glEnd();
    glmatrix.identity();
    glmatrix.transpose();
    glLoadMatrixd(glmatrix.getPointer());
    glBegin(GL_QUADS);
    glColor3f(0.7, 1, 1);
    glNormal3f(0, 1, 0);
    glVertex3f(-100, -5, -100);
    glVertex3f(100, -5, -100);
    glVertex3f(100, -5, 100);
    glVertex3f(-100, -5, 100);
    glEnd();
    

    glFlush();
    glutSwapBuffers();
}
void Window::idleCallback()
{
    Globals::cube.spin(1.0);   // rotate cube; if it spins too fast try smaller values and vice versa
    displayCallback();
}

void Window::processSpecialKeys(int key, int x, int y){
    //Display ball mode
    if(key == GLUT_KEY_F1){
    }
}
/**
 * Method for process keyboard actions
 **/
void Window::processNormalKeys(unsigned char key, int x, int y){
    if (key == 27){
        exit(0);
    }
}


