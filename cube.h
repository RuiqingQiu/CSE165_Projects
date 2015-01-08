#ifndef _CUBE_H_
#define _CUBE_H_

#include "Matrix4.h"

class Cube
{
  protected:
    Matrix4 model2world;            // model matrix (transforms model coordinates to world coordinates)
    double angle;                   // rotation angle [degrees]
    Vector4* position;
  public:
    Cube();   // Constructor
    Matrix4& getMatrix();
    void spin(double);      // spin cube [degrees]
    void moveLeft();
    void moveRight();
    void moveIn();
    void moveOut();
    void moveDown();
    void moveUp();
    void reset();
    void scaleUp();
    void scaleDown();
    void orbit(int);
    void fall();
    void print_position();
};

#endif

