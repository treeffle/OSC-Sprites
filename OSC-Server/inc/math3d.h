#ifndef _MATH3D_
#define _MATH3D_

#include <cmath>
#include "platform.h"
#ifdef RPI
    #include "GLES2/gl2.h"
#elif OS_IS_MACOSX
    #include <OpenGL/gl.h>
#else
    #include "GL/gl.h"
#endif

class Math3d
{
 public:
  static GLfloat *mat_mult_4x4(GLfloat*, GLfloat*);
  static GLfloat *mat_mult_3x3(GLfloat*, GLfloat*);
  static GLfloat *mat_mult_2x2(GLfloat*, GLfloat*);

  static GLfloat *mat_vec_mult_4x4(GLfloat*, GLfloat*);
  static GLfloat *mat_vec_mult_3x3(GLfloat*, GLfloat*);
  static GLfloat *mat_vec_mult_2x2(GLfloat*, GLfloat*);

  static GLfloat *vec_cross(GLfloat*,GLfloat*);
  static GLfloat vec_dot(int, GLfloat*, GLfloat*);
};

#endif
