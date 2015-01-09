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

namespace Globals
{
    extern Cube cube;
    extern btDiscreteDynamicsWorld* dynamicsWorld;
    extern btRigidBody* fallRigidBody;
    extern Camera* camera;
    extern MatrixTransform root;
};
void physics_setup();
void physics_cleanup();
void initWalls();


#endif /* defined(__CSE165_HW1__main__) */
