#include "Cube.h"
#include "Matrix4.h"

using namespace std;

Cube::Cube()
{
  angle = 0.0;
  position = new Vector4(0,0,0,1);
}

Matrix4& Cube::getMatrix()
{
    return model2world;
}

void Cube::spin(double deg)   // deg is in degrees
{
  angle += deg;
  if (angle > 360.0 || angle < -360.0)
      angle = 0.0;
  
  Matrix4 tmp = Matrix4();
  tmp.makeRotateY(deg);
  //tmp.makeRotateX(deg);
  //tmp.makeRotateZ (deg);
    //translate.makeTranslate(0.01, 0, 0);
  //m*R
  model2world = model2world * tmp; //* translate * tmp;
  //model2world.makeRotateY(angle);   // This creates the matrix to rotate the cube
  //model2world.makeRotate(angle, *new Vector3(1, 1, 1));
}
void Cube::moveLeft(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(-1, 0, 0);
    model2world = tmp * model2world;
    *position = tmp * *position;
}
void Cube::moveRight(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(1,0,0);
    //T*m
    model2world = tmp * model2world;
    *position = tmp * *position;

}
void Cube::moveIn(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(0, 0, 1);
    model2world = tmp * model2world;
    *position = tmp * *position;

}
void Cube::moveOut(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(0, 0, -1);
    model2world = tmp * model2world;
    *position = tmp * *position;

}
void Cube::moveUp(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(0, 1, 0);
    model2world = tmp * model2world;
    *position = tmp * *position;

}
void Cube::moveDown(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(0, -1, 0);
    model2world = tmp * model2world;
    *position = tmp * *position;

}
void Cube::reset(){
    delete position;
    position = new Vector4(0,0,0,1);
    model2world.identity();
}

void Cube::scaleUp(){
    Matrix4 tmp = Matrix4();
    tmp.makeScale(1.1, 1.1, 1.1);
    model2world = model2world * tmp;
}
void Cube::scaleDown(){
    Matrix4 tmp = Matrix4();
    tmp.makeScale(0.9, 0.9, 0.9);
    model2world = model2world * tmp;
}
void Cube::orbit(int direction){
    Matrix4 tmp = Matrix4();
    tmp.makeRotateZ(direction * 5);
    model2world = tmp * model2world;
}
void Cube::fall(){
    Matrix4 tmp = Matrix4();
    tmp.makeTranslate(0, -1, 0);
    model2world = tmp * model2world;
}

void Cube::print_position(){
    position->dehomogenize();
    Vector3* tmp = new Vector3(position->get_x(), position->get_y(), position->get_z());
    tmp->print("position is ");
}

