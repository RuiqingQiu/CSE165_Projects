//
//  Sphere.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Sphere__
#define __CSE167HW1__Sphere__

#include <stdio.h>
#include "Geode.h"

class Sphere : public Geode{
private:
    Vector3 color = Vector3(0, 1, 0);
public:
    Sphere();
    void render();
    void update();
    void BoundBox(Matrix4 C);
    void updateColor(Vector3 color_a);
};
#endif /* defined(__CSE167HW1__Sphere__) */
