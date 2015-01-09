//
//  Cube_G.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "CubeG.h"
#include "main.h"
CubeG::CubeG(){
    //center_c = Vector4(0, 0, 0, 1);
    center = Vector4(0, 0, 0, 1);
    scale = Vector4(1, 1, 1, 0);
    radius = sqrt(2.5*2.5 + 2.5*2.5);
}

void CubeG::render(){
    glColor3f(color.x, color.y, color.z);
    glutSolidCube(2);
}

void CubeG::update(){
    
}
void CubeG::updateColor(Vector3 color_a){
    color = color_a;
}
void CubeG::BoundBox(Matrix4 C){
        center = Vector4(0, 0, 0, 1);
        scale = Vector4(0.5, 0.5, 0.5, 0);
        Vector4 new_center = C * center;
        center = new_center;
        Vector4 new_scale = C * scale;
        scale = new_scale;
        double magnitude = new_scale.length();
        radius = magnitude;
   
        glPushMatrix();
        Matrix4 trans = Matrix4(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(new_center.x, new_center.y, new_center.z));
        Matrix4 scaling = Matrix4(Vector3(magnitude, 0, 0), Vector3(0, magnitude, 0), Vector3(0,0, magnitude), Vector3(0, 0, 0));
        Matrix4 glmatrix = trans * scaling;
        glmatrix.transpose();
        glLoadMatrixd(glmatrix.getPointer());
        glColor3f(color.x, color.y, color.z);
        glutWireSphere(1.2, 20, 20);
        glPopMatrix();
}
