//
//  Camera.cpp
//  CSE167HW1
//
//  Created by Ruiqing Qiu on 10/20/14.
//  Copyright (c) 2014 Ruiqing Qiu. All rights reserved.
//

#include "Camera.h"

Camera::Camera(){
    d = new Vector3(0,0,0);
    up = new Vector3(0, 1, 0);
    //e = new Vector3(0, 400, 20); //it shows up
    e = new Vector3(0,100,500);
    //e = new Vector3(0, 100, 300);
    //e = new Vector3(0, 5, 2);
    //e= new Vector3(0,-2,2);
    position = Vector4(0,0,0,0);
    C.identity();
}
//Return the matrix as a pointer to array of 16 elements
Matrix4& Camera::getMatrix(){
    //e->print("e");
    return C;
}
void Camera::update(){
    //e->print("E is ");
    //Getting the z-axis
    Vector3 z = *e - *d;
    z.normalize();
    //Getting the x-axis
    Vector3 x = (*up).cross(*up, z);
    x.normalize();
    //Getting the y-axis
    Vector3 y = (*up).cross(z, x);
    
    y.normalize();
    
    C = Matrix4(x, y, z, *e);
    
    Matrix4 rotation = Matrix4();
    rotation.identity();
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            rotation.m[i][j] = C.m[i][j];
        }
    }
    //Inverse of rotation matrix
    rotation.transpose();
    
    
    
    Matrix4 translation = Matrix4();
    translation.makeTranslate(-e->x, -e->y, -e->z);
    C = rotation * translation;
}

void Camera::xUp(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    //delete e;
    e = new Vector3(x+10, y, z);
    
}
void Camera::xDown(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    delete e;
    e = new Vector3(x-10, y, z);

    
}
void Camera::yUp(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    delete e;
    e = new Vector3(x, y+10, z);

}
void Camera::yDown(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    delete e;
    e = new Vector3(x, y-10, z);

}
void Camera::zUp(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    delete e;
    e = new Vector3(x, y, z+10);

}
void Camera::zDown(){
    float x = e->x;
    float y = e->y;
    float z = e->z;
    delete e;
    e = new Vector3(x, y, z-10);

}