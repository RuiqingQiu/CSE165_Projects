//
//  Geode.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Geode__
#define __CSE167HW1__Geode__

#include <stdio.h>
#include "Node.h"
#include <GLUT/glut.h>

class Geode: public Node{
public:
    void setC(Matrix4 C);
    void draw(Matrix4 C);
    void update();
    virtual void render() = 0;    
};
#endif /* defined(__CSE167HW1__Geode__) */
