//
//  Ball.cpp
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/8/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#include "Ball.h"
Ball::Ball(Vector3 color){
    world = new MatrixTransform();
    Sphere* piece = new Sphere();
    piece->updateColor(color);
    world->addChild(piece);
}
void Ball::draw(Matrix4 C){
    Matrix4 tmp = Matrix4();
    tmp.identity();
    world->draw(C);
}
void Ball::setLocation(float x, float y, float z){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(x, y, z);
    world->M = world->M * tmp;
}
