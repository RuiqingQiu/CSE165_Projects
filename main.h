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

namespace Globals
{
    extern Cube cube;
    extern btDiscreteDynamicsWorld* dynamicsWorld;
    extern btRigidBody* fallRigidBody;
    extern Camera* camera;
    extern MatrixTransform root;
    extern btSoftRigidDynamicsWorld* softworld;
    extern btSoftBodyWorldInfo	m_softBodyWorldInfo;
};
void physics_setup();
void physics_cleanup();
void initWalls();
btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape);
btRigidBody* createSphere(float rad, float x, float y, float z, float mass);

#endif /* defined(__CSE165_HW1__main__) */
