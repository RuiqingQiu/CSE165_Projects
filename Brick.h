//
//  Brick.h
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/8/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE165_HW1__Brick__
#define __CSE165_HW1__Brick__

#include <stdio.h>
#include "MatrixTransform.h"
#include "CubeG.h"
#include "main.h"
#include "btBulletDynamicsCommon.h"

#include <GLUT/glut.h>

class Brick{
public:
    MatrixTransform* world; //Position in the world
    btRigidBody* rb;
    btCollisionShape* shape;
    Vector3 m_color;
    float m_x;
    float m_mat[16];
    float t_mat[16];
    float m_y;
    float m_z;
    Brick(Vector3 color);
    void draw(Matrix4 C, float length);
    void draw2(btTransform t, btTransform b);
    void setLocation(float x, float y, float z);
    void physics(float x, float y, float z, float length, float m_mass);
    void print_height();
    bool isInfinite();
    int connection_index = -1;
    void setConnection(int c);
    int getConnection();
};
#endif /* defined(__CSE165_HW1__Brick__) */
