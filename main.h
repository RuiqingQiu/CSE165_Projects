//
//  main.h
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/7/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE165_HW1__main__
#define __CSE165_HW1__main__

#include <stdio.h>
#include "cube.h"
#include "btBulletDynamicsCommon.h"
#include "Camera.h"
#include <vector>
#include "MatrixTransform.h"
#include "Ball.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Leap.h"

using namespace Leap;
namespace Globals
{
    extern Cube cube;
    extern btDiscreteDynamicsWorld* dynamicsWorld;
    extern btRigidBody* fallRigidBody;
    extern Camera* camera;
    extern MatrixTransform root;
    extern btSoftRigidDynamicsWorld* softworld;
    extern btSoftBodyWorldInfo	m_softBodyWorldInfo;
    extern btGeneric6DofConstraint * joint_ball;
    // Use to activate/disable shadowShader
    extern GLhandleARB shadowShaderId;
    extern GLuint shadowMapUniform;
    // Z values will be rendered to this texture when using fboId framebuffer
    extern GLuint depthTextureId;
    // Hold id of the framebuffer for light POV rendering
    extern GLuint fboId;
    extern Vector3 light_pos;
    extern int homework_num;
    //extern LeapListener listener;
    extern int draw_mode;
    extern btCompoundShape* compound;
    extern btRigidBody* whole;
};
void physics_setup();
void physics_cleanup();
void initWalls();
bool shadow_setup();
btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape);
btRigidBody* createSphere(float rad, float x, float y, float z, float mass);

void initHw2();

#endif /* defined(__CSE165_HW1__main__) */
