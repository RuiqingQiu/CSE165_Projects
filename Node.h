//
//  Node.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Node__
#define __CSE167HW1__Node__

#include <stdio.h>
#include "Matrix4.h"
#include <GLUT/glut.h>


class Node
{

   
public:
    Vector4 center = Vector4(0, 0, 0, 1);
    Vector4 scale = Vector4(0.5, 0.5, 0.5, 0);
    float radius = 0;
    virtual void draw(Matrix4 C) = 0;
    virtual void update() = 0;
    virtual void BoundBox(Matrix4 C) = 0;
};
#endif /* defined(__CSE167HW1__Node__) */
