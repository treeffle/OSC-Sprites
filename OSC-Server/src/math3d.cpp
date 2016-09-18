
//everything is column major and returns a 1D array
//except for cross product which just returns a float

#include "math3d.h"

GLfloat* Math3d::mat_mult_4x4(GLfloat* A, GLfloat*B)
{
  GLfloat *C;
  C = new GLfloat[16];

  C[0] = A[0]*B[0] + A[4]*B[1] + A[8]*B[2] + A[12]*B[3];
  C[1] = A[1]*B[0] + A[5]*B[1] + A[9]*B[2] + A[13]*B[3];
  C[2] = A[2]*B[0] + A[6]*B[1] + A[10]*B[2] + A[14]*B[3];
  C[3] = A[3]*B[0] + A[7]*B[1] + A[11]*B[2] + A[15]*B[3];

  C[4] = A[0]*B[4] + A[4]*B[5] + A[8]*B[6] + A[12]*B[7];
  C[5] = A[1]*B[4] + A[5]*B[5] + A[9]*B[6] + A[13]*B[7];
  C[6] = A[2]*B[4] + A[6]*B[5] + A[10]*B[6] + A[14]*B[7];
  C[7] = A[3]*B[4] + A[7]*B[5] + A[11]*B[6] + A[15]*B[7];

  C[8] = A[0]*B[8] + A[4]*B[9] + A[8]*B[10] + A[12]*B[11];
  C[9] = A[1]*B[8] + A[5]*B[9] + A[9]*B[10] + A[13]*B[11];
  C[10] = A[2]*B[8] + A[6]*B[9] + A[10]*B[10] + A[14]*B[11];
  C[11] = A[3]*B[8] + A[7]*B[9] + A[11]*B[10] + A[15]*B[11];

  C[12] = A[0]*B[12] + A[4]*B[13] + A[8]*B[14] + A[12]*B[15];
  C[13] = A[1]*B[12] + A[5]*B[13] + A[9]*B[14] + A[13]*B[15];
  C[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
  C[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];

  return C;
}


GLfloat* Math3d::mat_mult_3x3(GLfloat* A, GLfloat*B)
{
  GLfloat *C;
  C = new GLfloat[9];

  C[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  C[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  C[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];
  C[3] = A[0]*B[3] + A[3]*B[4] + A[6]*B[5];
  C[4] = A[1]*B[3] + A[4]*B[4] + A[7]*B[5];
  C[5] = A[2]*B[3] + A[5]*B[4] + A[8]*B[5];
  C[6] = A[0]*B[6] + A[3]*B[7] + A[6]*B[8];
  C[7] = A[1]*B[6] + A[4]*B[7] + A[7]*B[8];
  C[8] = A[2]*B[6] + A[5]*B[7] + A[8]*B[8];

  return C;
}


GLfloat* Math3d::mat_mult_2x2(GLfloat* A, GLfloat*B)
{
  GLfloat *C;
  C = new GLfloat[4];

  C[0] = A[0]*B[0]+A[2]*B[1];
  C[1] = A[1]*B[0]+A[3]*B[1];
  C[2] = A[0]*B[2]+A[2]*B[3];
  C[3] = A[1]*B[2]+A[3]*B[3];

  return C;
}

GLfloat* Math3d::mat_vec_mult_4x4(GLfloat* A, GLfloat* B)
{
  GLfloat *C;
  C = new GLfloat[4];

  C[0] = A[0]*B[0] + A[4]*B[1] + A[8]*B[2] + A[12]*B[3];
  C[1] = A[1]*B[0] + A[5]*B[1] + A[9]*B[2] + A[13]*B[3];
  C[2] = A[2]*B[0] + A[6]*B[1] + A[10]*B[2] + A[14]*B[3];
  C[3] = A[3]*B[0] + A[7]*B[1] + A[11]*B[2] + A[15]*B[3];

  return C;
}

GLfloat* Math3d::mat_vec_mult_3x3(GLfloat* A, GLfloat* B)
{
  GLfloat *C;
  C = new GLfloat[3];

  C[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  C[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  C[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];

  return C;
}

GLfloat* Math3d::mat_vec_mult_2x2(GLfloat* A, GLfloat* B)
{
  GLfloat *C;
  C = new GLfloat[2];

  C[0] = A[0]*B[0] + A[2]*B[1];
  C[1] = A[1]*B[0] + A[3]*B[1];

  return C;
}

GLfloat* Math3d::vec_cross(GLfloat* A, GLfloat* B)
{
  GLfloat *C;
  C = new GLfloat[3];
  C[0] = A[1]*B[2]-A[2]*B[1];
  C[1] = A[2]*B[0]-A[0]*B[2];
  C[2] = A[0]*B[1]-A[1]*B[0];
  return C;
}

GLfloat Math3d::vec_dot(int n, GLfloat* A, GLfloat* B)
{
  GLfloat C;
  C = 0.f;

  for(int i=0; i<n; i++)
    C += (A[i]*B[i]);

  return C;
}
