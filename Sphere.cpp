//
//  Sphere.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Sphere.h"
#include "main.h"


Sphere::Sphere(){
    //center_c = Vector4(0, 0, 0, 1);
    center = Vector4(0, 0, 0, 1);
    scale = Vector4(0.5, 0.5, 0.5, 0);
    radius = 2;
}
void Sphere::render(){
    glColor3f(color.x, color.y, color.z);
    glutSolidSphere(1.0, 20.0, 20.0);
}

void Sphere::update(){
}
void Sphere::updateColor(Vector3 color_a){
    color = color_a;
}
void Sphere::BoundBox(Matrix4 C){
    //cout << Globals::boundOn << endl;
        center = Vector4(0, 0, 0, 1);
        scale = Vector4(0.5, 0.5, 0.5, 0);
        Vector4 new_center = C * center;
        Vector4 new_scale = C * scale;
        center = new_center;
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
