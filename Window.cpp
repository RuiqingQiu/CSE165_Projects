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
int Movement;
Vector3 lastPoint = Vector3(0, 0, 0);
Vector3 lastPoint_z = Vector3(0, 0, 0);


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
    //Globals::softworld->stepSimulation(1.0f/60.f,0);
    //tmp.print_height();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
    // Tell OpenGL what ModelView matrix to use:
    //gluLookAt(0, 15, 30, 0, 0, 0, 0, 1, 0);
    //gluLookAt(eye_x,eye_y,eye_z,0, 0, 0,0, 1, 0);
    Matrix4 glmatrix;
    glmatrix.identity();
    glmatrix = Globals::camera->getMatrix() * world;
    glmatrix.transpose();
    glLoadMatrixd(glmatrix.getPointer());
    glBegin(GL_QUADS);
    glColor3f(0.7, 1, 1);
    glNormal3f(0, 1, 0);
    glVertex3f(-100, -1, -100);
    glVertex3f(100, -1, -100);
    glVertex3f(100, -1, 100);
    glVertex3f(-100, -1, 100);
    glEnd();
    
    ball.draw(Globals::camera->getMatrix()*glmatrix*world);
    for(int i = 0; i < b_list.size(); i++){
        b_list[i].draw(Globals::camera->getMatrix()*glmatrix*world);
    }
    Globals::dynamicsWorld->debugDrawWorld();
    //Globals::softworld->debugDrawWorld();
    glmatrix.identity();
    glmatrix.transpose();
    
    //glLoadMatrixd(glmatrix.getPointer());
    
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
        Globals::camera->e->x = 0;
        Globals::camera->e->y = 15;
        Globals::camera->e->z = 30;
        Globals::camera->update();
        world.identity();
    }
    //move the camera y up
    else if(key == 'w'){
        Globals::camera->e->y+=10;
    }
    //move the camer y down
    else if(key == 's'){
        Globals::camera->e->y-=10;
    }
    //move the camera x up
    else if(key == 'd'){
        Globals::camera->e->x+=10;
    }
    //move the camera x down
    else if(key == 'a'){
        Globals::camera->e->x-=10;
    }
    else if(key == 'q'){
        Globals::camera->e->z-=10;
    }
    else if(key == 'e'){
        Globals::camera->e->z+=10;
    }
    Globals::camera->update();
}

Vector3 Window::trackBallMapping(int x, int y){
    Vector3 v = Vector3(0,0,0);
    float d;
    v.x = (2.0*x - width) / width;
    v.y = (height - 2.0*y) / height;
    v.z = 0.0;
    d = v.length();
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    v.normalize(); // Still need to normalize, since we only capped d, not v.
    return v;
}

void Window::mouseMove(int x, int y){
    Vector3 direction = Vector3(0, 0, 0);
    float pixel_diff;
    float rot_angle, zoom_factor;
    Vector3 curPoint = Vector3(0,0,0);
    lastPoint.print("last point is ");
    switch (Movement) {
        case 0:{
            cout << "case 0" << endl;
            curPoint = trackBallMapping(x, y);//Map the mouse position to a logical sphere location
            direction = curPoint - lastPoint;
            float velocity = direction.length();
            if(velocity > 0.0001){
                Vector3 rotAxis = lastPoint.cross(lastPoint, curPoint);
                rot_angle = velocity * 100;
                cout << "rot angle " << rot_angle << endl;
                GLfloat objectXform[16];
                glGetFloatv(GL_MODELVIEW_MATRIX, objectXform);
                glLoadIdentity();
                glRotatef(rot_angle, rotAxis.x, rotAxis.y, rotAxis.z);
                glMultMatrixf(objectXform);
                Matrix4 tmp = Matrix4();
                tmp.makeRotate(rot_angle, rotAxis);
                //Globals::bunny.getMatrix() = Globals::bunny.getMatrix() * tmp;
                world = tmp * world;
            }
            break;
        }
        case 1:
            cout << "case 1" << endl;
            pixel_diff = y - lastPoint_z.y;
            cout << "pixel diff " << pixel_diff << endl;
            zoom_factor = 1.0 + pixel_diff/50;
            cout << "zoom factor " << zoom_factor << endl;
            Matrix4 tmp = Matrix4();
            tmp.makeScale(zoom_factor, zoom_factor, zoom_factor);
            world = world * tmp;
            break;
    }
    lastPoint = curPoint;
    lastPoint_z = Vector3(x, y, 0);
}

void Window::mouse(int button, int state, int x, int y)
{
    switch(button){
            //Left-mouse button is being held down
        case GLUT_LEFT_BUTTON:
            Movement = 0;
            break;
        case GLUT_RIGHT_BUTTON:
            Movement = 1;
            break;
    }
    lastPoint = trackBallMapping(x, y);
    lastPoint_z = Vector3(x, y, 0);
    lastPoint.print("mouse");
    
    glMatrixMode(GL_MODELVIEW);
}


