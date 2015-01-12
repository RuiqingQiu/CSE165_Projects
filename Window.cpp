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
Ball Window::ball = Ball(Vector3(0,1,0));
vector<Brick> Window::b_list;

//for moving the camera
double eye_x = 0;
double eye_y = 15;
double eye_z = 30;

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
    clock_t startTime = clock();
    
    Globals::dynamicsWorld->stepSimulation(1 / 60.f, 10);

    //tmp.print_height();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
    // Tell OpenGL what ModelView matrix to use:
    //gluLookAt(0, 15, 30, 0, 0, 0, 0, 1, 0);
    gluLookAt(eye_x,eye_y,eye_z,
              0, 0, 0,
              0, 1, 0);


    glBegin(GL_QUADS);
    glColor3f(0.7, 1, 1);
    glNormal3f(0, 1, 0);
    glVertex3f(-100, -1, -100);
    glVertex3f(100, -1, -100);
    glVertex3f(100, -1, 100);
    glVertex3f(-100, -1, 100);
    glEnd();
    
    Matrix4 glmatrix;
    glmatrix.identity();
    ball.draw(Globals::camera->getMatrix()*glmatrix);
    for(int i = 0; i < b_list.size(); i++){
        b_list[i].draw(glmatrix);
    }
    
    glmatrix.identity();
    glmatrix.transpose();
    glLoadMatrixd(glmatrix.getPointer());
    
    glFlush();
    glutSwapBuffers();
    clock_t endTime = clock();
    cout << "frame rate: " << 1.0/(float((endTime - startTime))/CLOCKS_PER_SEC) << endl;
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
    else if(key == ' '){
        //physics_cleanup();
        physics_setup();
        initWalls();
    }
    //move the camera y up
    else if(key == 'w'){
        eye_y+=10;
    }
    //move the camer y down
    else if(key == 's'){
        eye_y-=10;
    }
    //move the camera x up
    else if(key == 'd'){
        eye_x+=10;
    }
    //move the camera x down
    else if(key == 'a'){
        eye_x-=10;
    }
    else if(key == 'q'){
        eye_z-=10;
    }
    else if(key == 'e'){
        eye_z+=10;
    }

}


