//
//  Cube_G.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Cube_G__
#define __CSE167HW1__Cube_G__

#include <stdio.h>
#include "Geode.h"
#include <math.h>
#include "Vector4.h"
#include "Matrix4.h"
class CubeG : public Geode{
private:
    Vector3 color = Vector3(0, 1, 0);
public:
    CubeG();
    void render();
    void update();
    void updateColor(Vector3 color_a);
    void BoundBox(Matrix4 C);

};
#endif /* defined(__CSE167HW1__Cube_G__) */
