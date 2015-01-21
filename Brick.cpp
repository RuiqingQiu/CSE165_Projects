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
    //CubeG* piece = new CubeG();
    //piece->updateColor(color);
    //world->addChild(piece);
    m_color = color;
}
void Brick::draw(Matrix4 C, float length){
    Matrix4 tmp = Matrix4();
    tmp.identity();
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    tmp.makeTranslate(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
    float mat[16];
    trans.getOpenGLMatrix(mat);
    tmp = Matrix4(
        Vector3(mat[0],mat[1],mat[2]),
        Vector3(mat[4],mat[5],mat[6]),
        Vector3(mat[8],mat[9],mat[10]),
        Vector3(mat[12],mat[13],mat[14])
                  );
    tmp = tmp * C;
    tmp.transpose();
    
  
    //glLoadMatrixf(mat);
    //glMultMatrixf(mat);	//multiplying the current matrix with it moves the object in place
    glPushMatrix();
    glMultMatrixf(mat);
    //glTranslatef(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
    glColor3f(m_color.getX(), m_color.getY(), m_color.getZ());
    glutSolidCube(length);
//    glMatrixMode(GL_TEXTURE);
//    glActiveTextureARB(GL_TEXTURE7);
//    glPushMatrix();
//    glMultMatrixf(mat);
//    //glTranslatef(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
//    glPopMatrix();
//    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void Brick::setLocation(float x, float y, float z){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(x, y, z);
    world->M = world->M * tmp;
}


void Brick::physics(float x, float y, float z, float length, float m_mass){
    btTransform t2;
    
    t2.setIdentity();
    
    t2.setOrigin(btVector3(x,y,z));
    btBoxShape* box = new btBoxShape(btVector3(length/2, length/2, length/2));
    //btCylinderShape* box = new btCylinderShape(btVector3(0.2f,0.2f,0.2f));
    btVector3 inertia(0,0,0);
    float mass = m_mass;
    box->calculateLocalInertia(mass,inertia);
    
    btDefaultMotionState* MotionState =
    new btDefaultMotionState(t2);
    btRigidBody::btRigidBodyConstructionInfo info(mass,MotionState,box,inertia); //motion state would actually be non-null in most real usages
    info.m_restitution = 0.5f;
    //info.m_friction = 1.5f;
    rb = new btRigidBody(info);
    Globals::dynamicsWorld->addRigidBody(rb);
}
void Brick::print_height(){
    btTransform trans;
    rb->getMotionState()->getWorldTransform(trans);
    cout << "current width is " << trans.getOrigin().getX() << endl;
    cout << "current height is " << trans.getOrigin().getY() << endl;
    cout << "current depth is " << trans.getOrigin().getZ() << endl;

}

