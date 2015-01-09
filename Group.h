//
//  Group.h
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#ifndef __CSE167HW1__Group__
#define __CSE167HW1__Group__

#include <stdio.h>
#include "Node.h"
#include <list>

using namespace std;
class Group : public Node{
protected:
    list<Node*> children;
public:
    Group();
    bool addChild(Node* child);
    bool removeChild(Node* child);
    void draw(Matrix4 C);
    void update();
    void BoundBox(Matrix4 C);

};
#endif /* defined(__CSE167HW1__Group__) */
