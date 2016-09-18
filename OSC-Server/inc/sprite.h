                                                 

#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>

#include "image.h"
#include "platform.h"
#ifdef RPI
    #include "GLES2/gl2.h"
    #include "EGL/egl.h"
    #include "EGL/eglext.h"
   #include <map>
#elif OS_IS_MACOSX
    #include "/usr/local/include/GL/glew.h"
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else
    #include "GL/glew.h"
    #include "GL/gl.h"
    #include "GL/glut.h"
#endif
#include <map>


#include "verts.h"

struct Container
{
  std::string vbo_name;
  std::string shader_name;
  std::string tex_name;
  GLfloat *transform_matrix;
  GLfloat *rgbcolor;
  bool has_texture;
  std::map<std::string,int> int_in;
  std::map<std::string,float> float_in;
  std::string instruction;
  std::string update;
};

class Sprite
{

private:
  std::string shader_name, texture_name, vbo_name;
  bool has_texture, recalculate_matrix, rotate_around_point;
  GLfloat rotate_point_x, rotate_point_y, rotate_point_z;
  GLfloat scale_x, scale_y, scale_z;
  GLfloat pos_x, pos_y, pos_z;
  GLfloat rot_x, rot_y, rot_z;

  GLfloat gradual_pos_dx, gradual_pos_dy, gradual_pos_dz;
  GLfloat gradual_scale_dx, gradual_scale_dy, gradual_scale_dz;
  GLfloat gradual_rot_dx, gradual_rot_dy, gradual_rot_dz;
  GLfloat goal_pos_x, goal_pos_y, goal_pos_z;
  GLfloat goal_scale_x, goal_scale_y, goal_scale_z;
  GLfloat goal_rot_x, goal_rot_y, goal_rot_z;

  int x, y, w, h, layer;
  int window_width, window_height;
  int gradual_dx, gradual_dy, gradual_dw, gradual_dh;
  int goal_x, goal_y, goal_w, goal_h;

  GLfloat gradual_dr, gradual_dg, gradual_db;
  GLfloat goal_r, goal_g, goal_b;
  GLfloat gradual_da;
  GLfloat goal_a;


  double last_time_drawn;

  bool is_gradual_translate;
  bool is_gradual_rotate;
  bool is_gradual_scale;
  bool is_gradual_position;
  bool is_gradual_size;
  bool is_gradual_color;
  bool is_gradual_fade;

  GLfloat *model_matrix;
  GLfloat *rgbcolor;
  GLfloat alpha;
  std::map<std::string, int> int_in;
  std::map<std::string, float> float_in;
  void calculate_matrix();
    
    
public:
  Sprite();
  ~Sprite(){};

  void  update_scale(GLfloat x, GLfloat y, GLfloat z);
  void  add_scale(GLfloat dx, GLfloat dy, GLfloat dz);
  void  gradual_scale(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec);

  void  update_size(int x_px, int y_px);
  void  add_size(int dx_px, int dy_px);
  void  gradual_size(int x_px, int y_px, GLfloat time_sec);

  void  update_translate(GLfloat x, GLfloat y, GLfloat z);
  void  add_translate(GLfloat dx, GLfloat dy, GLfloat dz);
  void  gradual_translate(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec);

  void  update_position(int x, int y, int layer);
  void  add_position(int dx, int dy);
  void  gradual_position(int x, int y, GLfloat time_sec);

  void  update_rotate(GLfloat x, GLfloat y, GLfloat z);
  void  add_rotate(GLfloat dx, GLfloat dy, GLfloat dz);
  void  gradual_rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec);

  void  update_color(GLfloat* rgb);
  void  update_color(GLfloat r, GLfloat g, GLfloat b);
  void  gradual_color(GLfloat r, GLfloat g, GLfloat b, GLfloat time_sec);

  void update_alpha(GLfloat a);
  void gradual_fade(GLfloat a, GLfloat time_sec);

  void  update_texture(std::string new_texture);
  void  update_shader(std::string new_shader);
  void  update_vbo(std::string new_vbo);
  void  update_int_in(std::map<std::string,int> new_int_in);
  void  update_float_in(std::map<std::string,float> new_float_in);

  void  update_rotate_point(GLfloat x, GLfloat y, GLfloat z);

  void  use_texture(bool b);
  void  use_rotate_point(bool b);

  std::string get_vbo_name();
  std::string get_shader_name();
  std::string get_texture_name();
  std::map<std::string, int> get_int_in();
  std::map<std::string, float> get_float_in();
  GLfloat* get_color();
  GLfloat get_alpha();
  void update_MM(double curr_time);
  GLfloat* get_MM();
  bool is_using_texture();

  Container convert_to_container();
};

#endif
