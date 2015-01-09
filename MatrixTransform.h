//
//  MatrixTransform.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__MatrixTransform__
#define __CSE167HW1__MatrixTransform__

#include <stdio.h>
#include "Node.h"
#include "Group.h"
class MatrixTransform : public Group{
public:
    bool isRoot;
    Matrix4 M;
    MatrixTransform();
    MatrixTransform(Matrix4 M);
    void draw(Matrix4 C);
    void update();
    void BoundBox(Matrix4 C);
};
#endif /* defined(__CSE167HW1__MatrixTransform__) */
