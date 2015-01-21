//
//  Cursor.h
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/20/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE165_HW1__Cursor__
#define __CSE165_HW1__Cursor__
#include <stdio.h>
#include "MatrixTransform.h"
#include "Sphere.h"
#include "main.h"

class Cursor{
public:
    btRigidBody* rb;
    Vector3 m_color;
    float radius;
    MatrixTransform* world; //Position in the world
    Cursor(Vector3 color, float rad);
    void draw(Matrix4 C, float length);
    void setLocation(float x, float y, float z);
    void physics(float x, float y, float z);
};
#endif /* defined(__CSE165_HW1__Cursor__) */
