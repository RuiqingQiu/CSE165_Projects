//
//  Ball.h
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/8/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE165_HW1__Ball__
#define __CSE165_HW1__Ball__

#include <stdio.h>
#include <stdio.h>
#include "MatrixTransform.h"
#include "Sphere.h"
class Ball{
public:
    MatrixTransform* world; //Position in the world
    Ball(Vector3 color);
    void draw(Matrix4 C);
    void setLocation(float x, float y, float z);
};
#endif /* defined(__CSE165_HW1__Ball__) */
