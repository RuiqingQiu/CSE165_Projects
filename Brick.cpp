//
//  Brick.cpp
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/8/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#include "Brick.h"
Brick::Brick(Vector3 color){
    world = new MatrixTransform();
    CubeG* piece = new CubeG();
    piece->updateColor(color);
    world->addChild(piece);
}
void Brick::draw(Matrix4 C){
    Matrix4 tmp = Matrix4();
    tmp.identity();
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    tmp.makeTranslate(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
    btMatrix3x3 rot = trans.getBasis();
    Matrix4 tmp1 = Matrix4();
    tmp1.identity();
    btVector3 x = rot.getColumn(0);
    btVector3 y = rot.getColumn(1);
    btVector3 z = rot.getColumn(2);
    //cout << x.getX() << " " << x.getY() << " " << x.getZ() << endl;
    //cout << y.getX() << " " << y.getY() << " " << y.getZ() << endl;
    //cout << z.getX() << " " << z.getY() << " " << z.getZ() << endl;
    tmp1 = Matrix4(Vector3(x.getX(), x.getY(), x.getZ()),
                   Vector3(y.getX(), y.getY(), y.getZ()),
                   Vector3(z.getX(), z.getY(), z.getZ()),
                   Vector3(0,0,0)
                   );
    world->draw(C * tmp * tmp1);
}
void Brick::setLocation(float x, float y, float z){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(x, y, z);
    world->M = world->M * tmp;
}

void Brick::physics(float x, float y, float z){
    btBoxShape* box = new btBoxShape(btVector3(1.0f,1.0f,1.0f));
    btVector3 inertia;
    float mass = 10.0f;
    box->calculateLocalInertia(mass,inertia);
    
    btDefaultMotionState* MotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z)));
    
    btRigidBody::btRigidBodyConstructionInfo info(mass,MotionState,box,inertia); //motion state would actually be non-null in most real usages
    //info.m_restitution = 1.3f;
    //info.m_friction = 1.5f;
    rb = new btRigidBody(info);
    Globals::dynamicsWorld->addRigidBody(rb);
}
void Brick::print_height(){
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    cout << "current height is " << trans.getOrigin().getY() << endl;
}

