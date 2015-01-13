//
//  main.cpp
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/7/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
// this is a new message
//it works
#include "main.h"
#include "Window.h"
#include <iostream>
#include <GLUT/glut.h>
#include "btBulletDynamicsCommon.h"
#include "GLDebugDrawer.h"
namespace Globals
{
    Cube cube;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btRigidBody* fallRigidBody;
    Camera* camera;
    MatrixTransform root;
}
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btCollisionShape* groundShape;
btRigidBody* groundRigidBody;

void physics_setup(){
    broadphase = new btDbvtBroadphase();
    
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    solver = new btSequentialImpulseConstraintSolver;
    
    //the world
    Globals::dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    
    Globals::dynamicsWorld->setGravity(btVector3(0, -10, 0));
    
    Globals::dynamicsWorld->setDebugDrawer(new GLDebugDrawer());
    Globals::dynamicsWorld->getDebugDrawer()->setDebugMode(1);
    groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    groundRigidBodyCI.m_friction = 1.5f;
    
    groundRigidBody = new btRigidBody(groundRigidBodyCI);
    groundRigidBody->setRestitution(0);
    Globals::dynamicsWorld->addRigidBody(groundRigidBody);
    
    
    
    
//    trans.setIdentity();
//    trans.setOrigin(btVector3(1,30,-5));
//    localCreateRigidBody( mass,trans,shape);
//    trans.setOrigin(btVector3(0,0,-5));
//    
//    btRigidBody* body0 = localCreateRigidBody( mass,trans,shape);
//    trans.setOrigin(btVector3(2*CUBE_HALF_EXTENTS,20,0));
//    mass = 1.f;
//    //	btRigidBody* body1 = 0;//localCreateRigidBody( mass,trans,shape);
//    btVector3 pivotInA(CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS,0);
//    btTypedConstraint* p2p = new btPoint2PointConstraint(*body0,pivotInA);
//    m_dynamicsWorld->addConstraint(p2p);
//    p2p ->setBreakingImpulseThreshold(10.2);
//    p2p->setDbgDrawSize(btScalar(5.f));
    
    
    //    btDefaultMotionState* fallMotionState =
    //    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
    //    btScalar mass = 1;
    //    btVector3 fallInertia(0, 0, 0);
    //    fallShape->calculateLocalInertia(mass, fallInertia);
    //    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    //    Globals::fallRigidBody = new btRigidBody(fallRigidBodyCI);
    //    Globals::fallRigidBody->setRestitution(0);
    //    Globals::dynamicsWorld->addRigidBody(Globals::fallRigidBody);
}

void physics_cleanup(){
    //Delete all the physics objects
    Globals::dynamicsWorld->removeRigidBody(Globals::fallRigidBody);
    delete Globals::fallRigidBody->getMotionState();
    delete Globals::fallRigidBody;
    
    Globals::dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;
    
    
    delete groundShape;
    
    
    delete Globals::dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
    

}
int main (int argc, char *argv[])
{
    srand (time(NULL));
    physics_setup();
        float specular[]  = {1.0, 1.0, 1.0, 1.0};
    float shininess[] = {100.0};
    
    glutInit(&argc, argv);      	      	      // initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
    glutInitWindowSize(Window::width, Window::height);      // set initial window size
    glutCreateWindow("CSE 165 Homework 1");    	      // open window and set window title
    //glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
    glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);             	      // set shading to smooth
    glMatrixMode(GL_PROJECTION);
    
    // Generate material properties:
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    // Generate light source:
    
    
    float position[]  = {0.0, 10.0, 10.0, 0.0};	// lightsource position
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glutDisplayFunc(Window::displayCallback);
    glutIdleFunc(Window::idleCallback);
    glutReshapeFunc(Window::reshapeCallback);
    glutMouseFunc(Window::mouse);
    glutMotionFunc(Window::mouseMove);

    glutSpecialFunc(Window::processSpecialKeys);
    glutKeyboardFunc(Window::processNormalKeys);
    Window::world.identity();
    Globals::cube.getMatrix().identity();
    Globals::camera = new Camera();
    Globals::camera->e->x = 0;
    Globals::camera->e->y = 15;
    Globals::camera->e->z = 30;
    Globals::camera->update();
    Globals::root.isRoot = true;
    //Init the wall
    initWalls();
    glutMainLoop();
    //delete everything used by the physics engine
    physics_cleanup();
    
    
    return 0;
}
void initWalls(){
    Window::b_list.clear();

    int row_max = 10;
    int col_max = 10;
    
    for(int i = 0; i < row_max; i++){
        for(int j = 0; j < col_max; j++){
            Brick b = Brick(Vector3(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX));
            if(j % 2 == 0){
                b.setLocation(-row_max+2*i + 0.5, j*2, 0);
                b.physics(-row_max+2*i + 0.5, j*2, 0);
            }
            else{
                b.setLocation(-row_max+2*i, j*2, 0);
                b.physics(-row_max+2*i, j*2, 0);
            }
            Window::b_list.push_back(b);
        }
    }
    Window::ball = Ball(Vector3(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX));
    Window::ball.setLocation(0, 20, 0);
    Window::ball.physics(0,20,0);
}