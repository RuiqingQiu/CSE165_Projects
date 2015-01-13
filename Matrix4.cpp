#include <math.h>

#include "Matrix4.h"

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
}

Matrix4::Matrix4(double n1){
    m[0][0] = 1;
    m[1][1] = 1;
    m[2][2] = 1;
    m[3][3] = 1;
}

Matrix4::Matrix4(Vector3 x, Vector3 y, Vector3 z, Vector3 e){
    m[0][0] = x.x;
    m[1][0] = x.y;
    m[2][0] = x.z;
    m[3][0] = 0;
 
    m[0][1] = y.x;
    m[1][1] = y.y;
    m[2][1] = y.z;
    m[3][1] = 0;
    
    m[0][2] = z.x;
    m[1][2] = z.y;
    m[2][2] = z.z;
    m[3][2] = 0;
    
    m[0][3] = e.x;
    m[1][3] = e.y;
    m[2][3] = e.z;
    m[3][3] = 1;
}

Matrix4::Matrix4(Vector4 x, Vector4 y, Vector4 z, Vector4 e){
    m[0][0] = x.get_x();
    m[1][0] = x.get_y();
    m[2][0] = x.get_z();
    m[3][0] = x.get_w();
    
    m[0][1] = y.get_x();
    m[1][1] = y.get_y();
    m[2][1] = y.get_z();
    m[3][1] = y.get_w();
    
    m[0][2] = z.get_x();
    m[1][2] = z.get_y();
    m[2][2] = z.get_z();
    m[3][2] = z.get_w();
    
    m[0][3] = e.get_x();
    m[1][3] = e.get_y();
    m[2][3] = e.get_z();
    m[3][3] = e.get_w();
}


Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
void Matrix4::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        m[i][j] = 1.0; 
      else 
        m[i][j] = 0.0;
    }
  }
}

// transpose the matrix (mirror at diagonal)
void Matrix4::transpose()
{
  Matrix4 temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      temp.m[j][i] = m[i][j];
    }
  }
  *this = temp;  // copy temporary values to this matrix
}

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4::makeRotateY(double angle)
{
    angle = angle / 180.0 * M_PI;  // convert from degrees to radians
    identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateX(double angle){
    angle = angle / 180.0 * M_PI;//Convert from degrees to radians
    identity();
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    
}// make rotation matrix about X axis with angle in degrees (note that the sin/cos functions in C++ expect radians so you need to convert to radians within the fmakeRotateZ(double angle);// make rotation matrix about Z axis with angle in degrees
void Matrix4::makeRotateZ(double angle){
    angle = angle / 180.0 * M_PI;//Convert from degrees to radians
    identity();
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
}

void Matrix4::makeRotate(double angle, const Vector3& axis){
    angle = angle / 180.0 * M_PI;//Convert from degrees to radians
    identity();
    float sinAngle, cosAngle;
    float mag = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    
    sinAngle = sinf (angle);
    cosAngle = cosf (angle);
    float xx, yy, zz, xy, yz, zx, xs, ys, zs;
    float oneMinusCos;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    x /= mag;
    y /= mag;
    z /= mag;
    
    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * sinAngle;
    ys = y * sinAngle;
    zs = z * sinAngle;
    oneMinusCos = 1.0f - cosAngle;
    
    m[0][0] = (oneMinusCos * xx) + cosAngle;
    m[0][1] = (oneMinusCos * xy) - zs;
    m[0][2] = (oneMinusCos * zx) + ys;
    m[0][3] = 0.0F;
    
    m[1][0] = (oneMinusCos * xy) + zs;
    m[1][1] = (oneMinusCos * yy) + cosAngle;
    m[1][2] = (oneMinusCos * yz) - xs;
    m[1][3] = 0.0F;
    
    m[2][0] = (oneMinusCos * zx) - ys;
    m[2][1] = (oneMinusCos * yz) + xs;
    m[2][2] = (oneMinusCos * zz) + cosAngle;
    m[2][3] = 0.0F;
    
    m[3][0] = 0.0F;
    m[3][1] = 0.0F;
    m[3][2] = 0.0F;
    m[3][3] = 1.0F;
}// Make a rotation matrix about an arbitrary axis

Matrix4 Matrix4::Inverse()
{
    //
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    //
    double Result[4][4];
    double tmp[12]; /* temp array for pairs */
    double src[16]; /* array of transpose source matrix */
    double det; /* determinant */
    /* transpose matrix */
    for (int i = 0; i < 4; i++)
    {
        src[i + 0 ] = m[i][0];
        src[i + 4 ] = m[i][1];
        src[i + 8 ] = m[i][2];
        src[i + 12] = m[i][3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    Result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    Result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    Result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    Result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    Result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    Result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    Result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    Result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    Result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    Result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    Result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    Result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    Result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    Result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    Result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    Result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];
    
    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    Result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    Result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    Result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    Result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    Result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    Result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    Result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    Result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    Result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    Result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    Result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    Result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    Result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    Result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    Result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    Result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det=src[0]*Result[0][0]+src[1]*Result[0][1]+src[2]*Result[0][2]+src[3]*Result[0][3];
    /* calculate matrix inverse */
    det = 1.0f / det;
    
    Matrix4 FloatResult;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            FloatResult.m[i][j] = float(Result[i][j] * det);
        }
    }
    return FloatResult;
    
    //
    // Inversion by LU decomposition, alternate implementation
    //
    /*int i, j, k;
     
     for (i = 1; i < 4; i++)
     {
     _Entries[0][i] /= _Entries[0][0];
     }
     
     for (i = 1; i < 4; i++)
     {
     for (j = i; j < 4; j++)
     {
     float sum = 0.0;
     for (k = 0; k < i; k++)
     {
     sum += _Entries[j][k] * _Entries[k][i];
     }
     _Entries[j][i] -= sum;
     }
     if (i == 4-1) continue;
     for (j=i+1; j < 4; j++)
     {
     float sum = 0.0;
     for (int k = 0; k < i; k++)
     sum += _Entries[i][k]*_Entries[k][j];
     _Entries[i][j] =
     (_Entries[i][j]-sum) / _Entries[i][i];
     }
     }
     
     //
     // Invert L
     //
     for ( i = 0; i < 4; i++ )
     {
     for ( int j = i; j < 4; j++ )
     {
     float x = 1.0;
     if ( i != j )
     {
     x = 0.0;
     for ( int k = i; k < j; k++ )
     x -= _Entries[j][k]*_Entries[k][i];
     }
     _Entries[j][i] = x / _Entries[j][j];
     }
     }
     
     //
     // Invert U
     //
     for ( i = 0; i < 4; i++ )
     {
     for ( j = i; j < 4; j++ )
     {
     if ( i == j ) continue;
     float sum = 0.0;
     for ( int k = i; k < j; k++ )
     sum += _Entries[k][j]*( (i==k) ? 1.0f : _Entries[i][k] );
     _Entries[i][j] = -sum;
     }
     }
     
     //
     // Final Inversion
     //
     for ( i = 0; i < 4; i++ )
     {
     for ( int j = 0; j < 4; j++ )
     {
     float sum = 0.0;
     for ( int k = ((i>j)?i:j); k < 4; k++ )
     sum += ((j==k)?1.0f:_Entries[j][k])*_Entries[k][i];
     _Entries[j][i] = sum;
     }
     }*/
}

Matrix4 Matrix4::operator*(const Matrix4& m2){
    Matrix4* product = new Matrix4();
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            for(int inner = 0; inner < 4; inner++){
                product->m[row][col] += this->m[row][inner]*m2.m[inner][col];
            }
        }
    }
    return *product;
}//multiply matrix with matrix

Vector4 Matrix4::operator*(const Vector4& v){
    double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
    x += this->m[0][0]*v.x;
    x += this->m[0][1]*v.y;
    x += this->m[0][2]*v.z;
    x += this->m[0][3]*v.w;
    
    y += this->m[1][0]*v.x;
    y += this->m[1][1]*v.y;
    y += this->m[1][2]*v.z;
    y += this->m[1][3]*v.w;
    
    
    z += this->m[2][0]*v.x;
    z += this->m[2][1]*v.y;
    z += this->m[2][2]*v.z;
    z += this->m[2][3]*v.w;
    
    w += this->m[3][0]*v.x;
    w += this->m[3][1]*v.y;
    w += this->m[3][2]*v.z;
    w += this->m[3][3]*v.w;
    
    return Vector4(x,y,z,w);
}//multiply matrix with vector



void Matrix4::makeScale(double sx, double sy, double sz){
    identity();
    this->m[0][0] = this->m[0][0] * sx;
    this->m[0][1] = this->m[0][1] * sx;
    this->m[0][2] = this->m[0][2] * sx;
    
    this->m[1][0] = this->m[1][0] * sy;
    this->m[1][1] = this->m[1][1] * sy;
    this->m[1][2] = this->m[1][2] * sy;
    
    this->m[2][0] = this->m[2][0] * sz;
    this->m[2][1] = this->m[2][1] * sz;
    this->m[2][2] = this->m[2][2] * sz;
}// make a non-uniform scaling matrix

void Matrix4::makeTranslate(double tx, double ty, double tz){
    identity();
    this->m[0][3] = tx;
    this->m[1][3] = ty;
    this->m[2][3] = tz;
}// make a translation matrix
void Matrix4::print(string comment){
    cout << comment << endl;
    for(int row = 0; row < 4; row++){
        cout << this->m[row][0] << ", " << this->m[row][1] << ", " << this->m[row][2] << ", " << this->m[row][3] << endl;
    }
}// print the matrix (display all 16 matrix components numerically on the screen in a 4x4 array)

void Matrix4::resetTranslate(){
    getPointer()[12] = 0;
    getPointer()[13] = 0;
    getPointer()[14] = 0;
}

float invf(int i,int j,const float* m){
    
    int o = 2+(j-i);
    
    i += 4+o;
    j += 4-o;
    
#define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
    
    float inv =
    + e(+1,-1)*e(+0,+0)*e(-1,+1)
    + e(+1,+1)*e(+0,-1)*e(-1,+0)
    + e(-1,-1)*e(+1,+0)*e(+0,+1)
    - e(-1,-1)*e(+0,+0)*e(+1,+1)
    - e(-1,+1)*e(+0,-1)*e(+1,+0)
    - e(+1,-1)*e(-1,+0)*e(+0,+1);
    
    return (o%2)?inv : -inv;
    
#undef e
    
}

Matrix4 Matrix4::inverseMatrix4x4(const float *m)
{
    
    float inv[16];
    
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            inv[j*4+i] = invf(i,j,m);
    
    double D = 0;
    
    for(int k=0;k<4;k++) D += m[k] * inv[k*4];
    
    if (D == 0) return false;
    
    D = 1.0 / D;
    float out[16];
    for (int i = 0; i < 16; i++)
        out[i] = inv[i] * D;
    Matrix4 tmp = Matrix4();
    tmp.m[0][0] = out[0];
    tmp.m[0][1] = out[1];
    tmp.m[0][2] = out[2];
    tmp.m[0][3] = out[3];
    
    tmp.m[1][0] = out[4];
    tmp.m[1][1] = out[5];
    tmp.m[1][2] = out[6];
    tmp.m[1][3] = out[7];
    
    tmp.m[2][0] = out[8];
    tmp.m[2][1] = out[9];
    tmp.m[2][2] = out[10];
    tmp.m[2][3] = out[11];
    
    tmp.m[3][0] = out[12];
    tmp.m[3][1] = out[13];
    tmp.m[3][2] = out[14];
    tmp.m[3][3] = out[15];

    return tmp;
    
}

