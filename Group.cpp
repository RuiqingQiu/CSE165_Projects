//
//  Group.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Group.h"

Group::Group(){
    
}
bool Group::addChild(Node* child){
    children.push_back(child);
    return true;
}

bool Group::removeChild(Node* child){
    children.remove(child);
    return true;
}

void Group::draw(Matrix4 C){
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        (*it)->draw(C);
    }
}
void Group::update(){
    
}
void Group::BoundBox(Matrix4 C){
    /*for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        //cout << "in here" << endl;
        (*it)->BoundBox(C);
    }
    */
}
