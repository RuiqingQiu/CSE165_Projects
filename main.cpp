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
#include <OpenGL/OpenGL.h>
#include "btBulletDynamicsCommon.h"
#include "GLDebugDrawer.h"
#include <string.h>
using namespace Leap;

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
    // Use to activate/disable shadowShader
    GLhandleARB shadowShaderId;
    GLuint shadowMapUniform;
    // Z values will be rendered to this texture when using fboId framebuffer
    GLuint depthTextureId;
    // Hold id of the framebuffer for light POV rendering
    GLuint fboId;
    Vector3 light_pos = Vector3(0,100,0);
    int homework_num = 2;
    //LeapListener listener;
    int draw_mode = 0; // 0 for no draw, 1 for draw
}

//1 for homework 1
//2 for homework 2
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
/**PHYSICS END **/


// Loading shader function
GLhandleARB loadShader(char* filename, unsigned int type)
{
    FILE *pfile;
    GLhandleARB handle;
    const GLcharARB* files[1];
    
    // shader Compilation variable
    GLint result;				// Compilation code result
    GLint errorLoglength ;
    char* errorLogText;
    GLsizei actualErrorLogLength;
    
    char buffer[400000];
    
    // This will raise a warning on MS compiler
    pfile = fopen(filename, "rb");
    if(!pfile)
    {
        printf("Sorry, can't open file: '%s'.\n", filename);
    }
    
    fread(buffer,sizeof(char),400000,pfile);
    //printf("%s\n",buffer);
    
    
    fclose(pfile);
    
    handle = glCreateShaderObjectARB(type);
    if (!handle)
    {
        //We have failed creating the vertex shader object.
        printf("Failed creating vertex shader object from file: %s.",filename);
    }
    
    files[0] = (const GLcharARB*)buffer;
    glShaderSourceARB(
                      handle, //The handle to our shader
                      1, //The number of files.
                      files, //An array of const char * data, which represents the source code of theshaders
                      NULL);
    
    glCompileShaderARB(handle);
    
    //Compilation checking.
    glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
    
    // If an error was detected.
    if (!result)
    {
        //We failed to compile.
        printf("Shader '%s' failed compilation.\n",filename);
        
        //Attempt to get the length of our error log.
        glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
        
    }
    
    return handle;
}

void loadShadowShader()
{
    GLhandleARB vertexShaderHandle;
    GLhandleARB fragmentShaderHandle;
    
    vertexShaderHandle   = loadShader("/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/CSE165 HW1/VertexShader",GL_VERTEX_SHADER);
    fragmentShaderHandle = loadShader("/Users/ruiqingqiu/Desktop/Qiu_Code/CSE165/CSE165 HW1/FragmentShader",GL_FRAGMENT_SHADER);
    
    Globals::shadowShaderId = glCreateProgramObjectARB();
    
    glAttachObjectARB(Globals::shadowShaderId,vertexShaderHandle);
    glAttachObjectARB(Globals::shadowShaderId,fragmentShaderHandle);
    glLinkProgramARB(Globals::shadowShaderId);
    
    Globals::shadowMapUniform = glGetUniformLocationARB(Globals::shadowShaderId,"ShadowMap");
}

void generateShadowFBO()
{
    int shadowMapWidth = Window::width * 4;
    int shadowMapHeight = Window::height * 4;
    
    //GLfloat borderColor[4] = {0,0,0,0};
    
    GLenum FBOstatus;
    
    // Try to use a texture depth component
    glGenTextures(1, &Globals::depthTextureId);
    glBindTexture(GL_TEXTURE_2D, Globals::depthTextureId);
    
    // GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // Remove artefact on the edges of the shadowmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    
    //glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
    
    
    
    // No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // create a framebuffer object
    glGenFramebuffersEXT(1, &Globals::fboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Globals::fboId);
    
    // Instruct openGL that we won't bind a color texture with the currently binded FBO
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // attach the texture to FBO depth attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, Globals::depthTextureId, 0);
    
    // check FBO status
    FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
    printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
    
    // switch back to window-system-provided framebuffer
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}



int main (int argc, char *argv[])
{
    // Create a sample listener and controller
    Controller controller;
    
    // Have the sample listener receive events from the controller
    controller.addListener(Window::listener);
    
    if (argc > 1 && strcmp(argv[1], "--bg") == 0)
        controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    
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
    
    
    float position[]  = {0, 100, 0, 0.0};// lightsource position
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
    
    generateShadowFBO();
    loadShadowShader();
    // This is important, if not here, FBO's depthbuffer won't be populated.
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1.0f);
    
    glEnable(GL_CULL_FACE);
    //glutDisplayFunc(renderScene);
    //glutIdleFunc(renderScene);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    
    
    if(Globals::homework_num == 1){
        //Init the wall
        initWalls();
    }
    else if (Globals::homework_num == 2){
        initHw2();
    }
    
    glutMainLoop();
    //delete everything used by the physics engine
    physics_cleanup();
    // Remove the sample listener when done
    controller.removeListener(Window::listener);

    return 0;
}

void initHw2(){
    Window::cursor = Cursor(Vector3(float(rand())/ RAND_MAX, float(rand())/ RAND_MAX, float(rand())/ RAND_MAX), 1);
    Window::cursor.setLocation(0, 0, 0);
    Window::cursor.physics(0,0,0);
}


void initWalls(){
    Window::b_list.clear();
    Window::bodies.clear();

    int row_max = 20;
    int col_max = 20;
    
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

