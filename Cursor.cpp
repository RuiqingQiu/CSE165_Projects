//
//  Cursor.cpp
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/20/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#include "Cursor.h"
Cursor::Cursor(Vector3 color,float rad){
    world = new MatrixTransform();
    Sphere* piece = new Sphere();
    piece->updateColor(color);
    world->addChild(piece);
    m_color = color;
    radius = rad;
}
void Cursor::draw(Matrix4 C, float length){
    Matrix4 tmp = Matrix4();
    tmp.identity();
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    tmp.makeTranslate(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
    float mat[16];
    trans.getOpenGLMatrix(mat);
    
    glPushMatrix();
    C.transpose();
    //glMultMatrixd(C.getPointer());
    //glLoadMatrixd(C.getPointer());
    glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    glColor3f(m_color.getX(), m_color.getY(), m_color.getZ());
    glutSolidSphere(length,20,20);
    //    glMatrixMode(GL_TEXTURE);
    //    glActiveTextureARB(GL_TEXTURE7);
    //    glPushMatrix();
    //    glMultMatrixf(mat);
    //    glPopMatrix();
    //    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}
void Cursor::setLocation(float x, float y, float z){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(x, y, z);
    world->M = world->M * tmp;
}
void Cursor::physics(float x, float y, float z){
    btSphereShape* sphereShape = new btSphereShape(radius);
    //btCylinderShape* box = new btCylinderShape(btVector3(0.2f,0.2f,0.2f));
    btVector3 inertia;
    //Cursor has 0 mass
    float mass = 0.0f;
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
