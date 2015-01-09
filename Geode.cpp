//
//  Geode.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Geode.h"
#include "main.h"

void Geode::setC(Matrix4 C){
    glMatrixMode(GL_MODELVIEW);
    C.transpose();
    glLoadMatrixd(C.getPointer());
    C.transpose();
}
void Geode::draw(Matrix4 C){
    glMatrixMode(GL_MODELVIEW);
    C.transpose();
    glLoadMatrixd(C.getPointer());
    C.transpose();
    render();
}
void Geode::update(){
    
}
