//
//  MatrixTransform.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 11/10/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "MatrixTransform.h"
#include "main.h"
MatrixTransform::MatrixTransform(){
    M = Matrix4();
    M.identity();
}
MatrixTransform::MatrixTransform(Matrix4 M){
    
}
void MatrixTransform::draw(Matrix4 C){
    M.print("M ");
    Matrix4 C_new = C * M;
    C_new.print("C new" );
    if(isRoot){
        for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
            (*it)->draw(C_new);
        }

        return;
    }
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        (*it)->draw(C_new);
    }

}
void MatrixTransform::BoundBox(Matrix4 C){
    
    double x_max = -10000;
    double x_min = 10000;
    double y_max = -10000;
    double y_min = 10000;
    double z_max = -10000;
    double z_min = 10000;
    Matrix4 C_new = C * M;
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        //cout << "in here" << endl;
        (*it)->BoundBox(C_new);
    }
    for (std::list<Node*>::iterator it = children.begin(); it != children.end(); it++){
        if((*it)->center.x + radius > x_max){
            //cout << "update" << endl;
            x_max = (*it)->center.x + (*it)->radius;
        }
        if((*it)->center.x - (*it)->radius < x_min){
            x_min = (*it)->center.x + (*it)->radius;
        }
        if((*it)->center.y + (*it)->radius > y_max){
            y_max = (*it)->center.y + (*it)->radius;
        }
        if((*it)->center.y - (*it)->radius < y_min){
            y_min = (*it)->center.y - (*it)->radius;
        }
        if((*it)->center.z + (*it)->radius > z_max){
            z_max = (*it)->center.z + (*it)->radius;
        }
        if((*it)->center.z - (*it)->radius < z_min){
            z_min = (*it)->center.z - (*it)->radius;
        }
    }
    //cout << x_max << " " << x_min << " " << y_max << " " << y_min << " " << z_max << " " << z_min << endl;
    if(x_max - x_min > y_max - y_min){
        radius = x_max - x_min;
    }
    else{
        radius = y_max - y_min;
    }
    if(z_max - z_min > radius){
        radius = z_max - z_min;
    }
    
    center = Vector4(0, 0, 0, 1);
    Vector4 new_center = C_new * center;
    center = new_center;
    //cout << children.size() << endl;
    //cout << isRoot << endl;
    if(isRoot){
        return;
    }

    glPushMatrix();
    Matrix4 trans = Matrix4(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(new_center.x, new_center.y, new_center.z));
    Matrix4 scaling = Matrix4(Vector3(radius/2, 0, 0), Vector3(0, radius/2, 0), Vector3(0,0, radius/2), Vector3(0, 0, 0));
    //scaling.print("scale");
    Matrix4 glmatrix = trans * scaling;
    glmatrix.transpose();
    glLoadMatrixd(glmatrix.getPointer());
    glColor3f(0.2, 0.5, 0.5);
            
    glutWireSphere(1, 10, 10);
    glPopMatrix();
}

void MatrixTransform::update(){
    
}
