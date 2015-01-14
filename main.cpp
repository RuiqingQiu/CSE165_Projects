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
    btSoftRigidDynamicsWorld* softworld;
    btSoftBodyWorldInfo	m_softBodyWorldInfo;
    vector<btRigidBody*> bodies;
    btGeneric6DofConstraint * joint_ball;
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
    ///register some softbody collision algorithms on top of the default btDefaultCollisionConfiguration
    btSoftBodyRigidBodyCollisionConfiguration *m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    
    btCollisionDispatcher *m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
    Globals::m_softBodyWorldInfo.m_dispatcher = m_dispatcher;
    Globals::m_softBodyWorldInfo.m_broadphase = broadphase;
    Globals::m_softBodyWorldInfo.m_gravity.setValue(0,-10,0);
    Globals::m_softBodyWorldInfo.m_sparsesdf.Initialize();
    
    btConstraintSolver*	m_solver = new btSequentialImpulseConstraintSolver;
    Globals::softworld = new btSoftRigidDynamicsWorld(m_dispatcher,broadphase,m_solver,m_collisionConfiguration);
    Globals::softworld->setGravity(btVector3(0, -10, 0));
    Globals::softworld->setDebugDrawer(new GLDebugDrawer());
    Globals::softworld->getDebugDrawer()->setDebugMode(1);
    Globals::dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    //Globals::dynamicsWorld =new btSoftRigidDynamicsWorld(m_dispatcher,broadphase,solver,m_collisionConfiguration);
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
    //Window::bodies.push_back(groundRigidBody);
 

    //TRACEDEMO
//    const int n=15;
//    for(int i=0;i<n;++i)
//    {
//        btSoftBody* psb=btSoftBodyHelpers::CreateRope(Globals::m_softBodyWorldInfo,
//                                                      btVector3(-10,0,i*0.25+10),
//                                                      btVector3(10,0,i*0.25+10),
//                                                      16,
//                                                      1+2);
//        psb->m_cfg.piterations		=	4;
//        psb->m_materials[0]->m_kLST	=	0.1+(i/(btScalar)(n-1))*0.9;
//        psb->setTotalMass(20);
//        Globals::softworld->addSoftBody(psb);
//    }

    
    
    

    
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

btRigidBody* createSphere(float rad, float x, float y, float z, float mass)

{
    
    btTransform t2;
    
    t2.setIdentity();
    
    t2.setOrigin(btVector3(x,y,z));
    
    //btBoxShape* sphere2 = new btBoxShape(btVector3(rad,rad,rad));//rad
    btSphereShape* sphere2 = new btSphereShape(rad);//rad

    btVector3 inertia(0,0,0);
    
    sphere2->calculateLocalInertia(mass, inertia);
    
    btMotionState* motion2 = new btDefaultMotionState(t2);
    
    btRigidBody::btRigidBodyConstructionInfo info2(mass, motion2, sphere2,inertia);//mass
    
    //    btStaticPlaneShape* sphere2 = new btStaticPlaneShape(btVector3(0,1,0),0);
    
    //    btMotionState* motion2 = new btDefaultMotionState(t);
    
    //    btRigidBody::btRigidBodyConstructionInfo info2(0.0, motion2, sphere2);
    
    info2.m_restitution = 0.5;
    
    btRigidBody* sphere = new btRigidBody(info2);
    
    //sphere-
    
    //m_sphere = sphere;
    
    Globals::dynamicsWorld->addRigidBody(sphere);
    
    //sphere->applyImpulse(btVector3(0.1,0,0), btVector3(0,0,0));
    
    
    
    
    
    //bodies.push_back(sphere);
    
    
    
    return sphere;
    
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
    Window::bodies.clear();

    int row_max = 10;
    int col_max = 10;
    
    for(int i = 0; i < row_max; i++){
        for(int j = 0; j < col_max; j++){
            Brick b = Brick(Vector3(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX));
            if(j % 2 == 0){
                b.setLocation(-row_max+2*i + 0.5, j*2, 0);
                b.physics(-row_max+2*i + 0.5, j*2, 0, 2,1);
            }
            else{
                b.setLocation(-row_max+2*i, j*2, 0);
                b.physics(-row_max+2*i, j*2, 0, 2,1);
            }
            Window::b_list.push_back(b);
        }
    }
    
    btRigidBody* s1 = createSphere(0.2, 0, 25, 10, 0);
    Window::bodies.push_back(s1);
//    Brick b = Brick(Vector3(1,1,0));
//    b.setLocation(0, 25, 10);
//    b.physics(0, 25, 10,0.2,0);
//    Window::rope_list.push_back(b);
    btRigidBody* s2 = nullptr;
    for(int i = 0; i < 20; i++){
//        Brick b2 = Brick(Vector3(1,1,0));
//        b2.setLocation(0, 25-0.4*i, 10);
//        b2.physics(0, 25-0.4*i,10,0.2,1);
//        Window::rope_list.push_back(b2);

        s2 = createSphere(0.2, 0, 25-0.4*i, 10, 1);
        Window::bodies.push_back(s2);
        btGeneric6DofConstraint * joint6DOF;
        
        btTransform localA, localB, toground;
        
        bool useLinearReferenceFrameA = true;
        
        
        
        localA.setIdentity(); localB.setIdentity();
        
        
        
        localA.setOrigin(btVector3(0,-0.2,0));
        
        
        
        localB.setOrigin(btVector3(0,0.2,0));
        
        
        
        //toground.setOrigin(btVector3(0,-20,-5));
        
        
        
        //joint6DOF = new btGeneric6DofConstraint(*(b.rb), *(b2.rb), localA, localB,useLinearReferenceFrameA);
        joint6DOF = new btGeneric6DofConstraint(*s1, *s2, localA, localB,useLinearReferenceFrameA);
        joint6DOF->setLinearLowerLimit(btVector3(0,0,0));
        joint6DOF->setLinearUpperLimit(btVector3(0,0,0));
        joint6DOF->setAngularLowerLimit(btVector3(0,0,0));
        joint6DOF->setAngularUpperLimit(btVector3(0,0,0));
        Globals::dynamicsWorld->addConstraint(joint6DOF);
        s1 = s2;
        //b = b2;
    }
    
    Window::ball = Ball(Vector3(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX), 2);
    Window::ball.setLocation(0, 10, 10);
    Window::ball.physics(0,10,10);

    
    btTransform localA, localB, toground;
    
    bool useLinearReferenceFrameA = true;
    
    
    
    localA.setIdentity(); localB.setIdentity();
    
    
    
    localA.setOrigin(btVector3(0,-0.2,0));
    
    
    
    localB.setOrigin(btVector3(0,1,0));
    
    
    //toground.setOrigin(btVector3(0,-20,-5));
    
    
    
    //joint6DOF = new btGeneric6DofConstraint(*(b.rb), *(Window::ball.rb), localA, localB,useLinearReferenceFrameA);
    Globals::joint_ball = new btGeneric6DofConstraint(*s1, *(Window::ball.rb), localA, localB,useLinearReferenceFrameA);
    
    
    
    Globals::dynamicsWorld->addConstraint(Globals::joint_ball);
    
    
}

btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape)
{
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        shape->calculateLocalInertia(mass,localInertia);
    
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    
#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    
    btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);
    
    btRigidBody* body = new btRigidBody(cInfo);
    //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
    
#else
    btRigidBody* body = new btRigidBody(mass,0,shape,localInertia);
    body->setWorldTransform(startTransform);
#endif//
    
    Globals::dynamicsWorld->addRigidBody(body);
    
    return body;
}