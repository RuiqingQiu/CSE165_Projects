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
    m_color = color;

}
void Ball::draw(Matrix4 C){
    Matrix4 tmp = Matrix4();
    tmp.identity();
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    tmp.makeTranslate(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
    float mat[16];
    trans.getOpenGLMatrix(mat);

    glPushMatrix();
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    glColor3f(m_color.getX(), m_color.getY(), m_color.getZ());
    glutSolidSphere(1,20,20);
    glPopMatrix();
}
void Ball::setLocation(float x, float y, float z){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(x, y, z);
    world->M = world->M * tmp;
}
void Ball::physics(float x, float y, float z){
    btSphereShape* sphereShape = new btSphereShape(1);
    //btCylinderShape* box = new btCylinderShape(btVector3(0.2f,0.2f,0.2f));
    btVector3 inertia;
    float mass = 10.0f;
    sphereShape->calculateLocalInertia(mass,inertia);
    
    btDefaultMotionState* MotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z)));
    btRigidBody::btRigidBodyConstructionInfo info(mass,MotionState,sphereShape,inertia); //motion state would actually be non-null in most real usages
    //info.m_restitution = 1.3f;
    //info.m_friction = 1.5f;
    rb = new btRigidBody(info);
    //rb->applyForce(btVector3(0,5,5), btVector3(x,y,z));
    //rb->setAngularVelocity(btVector3(0,5,5));
    //rb->setLinearVelocity(btVector3(0,0,-5));
    Globals::dynamicsWorld->addRigidBody(rb);

}
