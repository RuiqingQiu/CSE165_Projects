//
//  Window.h
//  CSE167 Final Project
//
//  Created by Ruiqing Qiu on 12/7/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167_Final_Project__Window__
#define __CSE167_Final_Project__Window__

#include <stdio.h>
#include <GLUT/glut.h>
#include "Matrix4.h"
#include "CubeG.h"
#include "main.h"
#include "Brick.h"
#include "Cursor.h"
#include "LeapListener.h"

class Window	  // OpenGL output window related routines
{
public:
    static int width, height; 	            // window size
    static Matrix4 world;
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
    static void processSpecialKeys(int key, int x, int y);
    static void processNormalKeys(unsigned char key, int x, int y);
    static void shadow_set_up();
    static int LoadGLTextures();
    static void mouse(int button, int state, int x, int y);
    static void mouseMove(int x, int y);
    static Vector3 trackBallMapping(int x, int y);
    static Ball ball;
    static vector<Brick> b_list;
    //Draw function for homework 1
    static void draw();
    
    static vector<Brick> rope_list;
    static vector<btRigidBody*> bodies;
    static void renderSphere(btRigidBody* body);
    static void updateCamera();
    static void renderPlane(btRigidBody* body);
    static void renderCube(btRigidBody* body);
    
    //Draw function for homework 2
    static void draw2();
    static void draw3();
    static Cursor cursor;
    static LeapListener listener;
    static void start_physics();
};
#endif /* defined(__CSE167_Final_Project__Window__) */
