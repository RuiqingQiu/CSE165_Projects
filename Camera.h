//
//  Camera.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 10/20/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Camera__
#define __CSE167HW1__Camera__

#include <stdio.h>
#include "Vector3.h"
#include "Matrix4.h"

class Camera
{
public:
    //center of projection
    Vector3 *e;
    //look at point
    Vector3 *d;
    //up vector
    Vector3 *up;
    Vector4 position = Vector4(0,0,0,0);
    //Interal camera matrix
    Matrix4 C;
    Camera();//Constructor
    Matrix4& getMatrix();
    void update();
    void xUp();
    void xDown();
    void yUp();
    void yDown();
    void zUp();
    void zDown();
};
#endif /* defined(__CSE167HW1__Camera__) */
