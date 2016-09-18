

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cmath>
#include <sys/time.h>

#include "sprite.h"
#include "math3d.h"

Sprite::Sprite(){ 
    rgbcolor = new GLfloat[4];
    rgbcolor[0] = 0.5;
    rgbcolor[1] = 0.0;
    rgbcolor[2] = rgbcolor[3] = 1.0;
    alpha = 1.0;
    shader_name = "standard"; 
    scale_x = scale_y = scale_z = 1.0; 
    pos_x = pos_y = pos_z = 0.0; 
    rot_x = rot_y = rot_z = 0;
    x = y  = w = h = layer = 0; 
    has_texture = false; 
    is_gradual_translate = is_gradual_rotate = is_gradual_scale = false;
    is_gradual_size = is_gradual_position = is_gradual_color = is_gradual_fade = false;
    last_time_drawn = -1.0;
    recalculate_matrix = true;
    rotate_around_point = false;

    //screw it i'm storing this here
    window_width = 768*3 + 2*768/15.4*1.235*2;
    window_height = 1366;
}



/*
    model matrix calculation
*/

void Sprite::calculate_matrix()
{
    float cx = cos(rot_x * M_PI/180.0);
    float tx = -sin(rot_x * M_PI/180.0);
    float sx = sin(rot_x * M_PI/180.0);
    float cy = cos(rot_y * M_PI/180.0);
    float ty = -sin(rot_y * M_PI/180.0);
    float sy = sin(rot_y * M_PI/180.0);
    float cz = cos(rot_z * M_PI/180.0);
    float tz = -sin(rot_z * M_PI/180.0);
    float sz = sin(rot_z * M_PI/180.0);
    GLfloat translate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, pos_x, pos_y, pos_z, 1};
    GLfloat scale[16] = {scale_x,0,0,0, 0,scale_y,0,0, 0,0,scale_z,0, 0,0,0,1};
    GLfloat rotate_x[16] = {1.0,0.0,0.0,0.0, 0.0,cx,sx,0.0, 0.0,tx,cx,0.0, 0.0,0.0,0.0,1.0};
    GLfloat rotate_y[16] = {cy,0.0,ty,0.0, 0.0,1.0,0.0,0.0, sy,0.0,cy,0.0, 0.0,0.0,0.0,1.0};
    GLfloat rotate_z[16] = {cz,sz,0.0,0.0, tz,cz,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0};
   
    if(rotate_around_point)
    {
        model_matrix = Math3d::mat_mult_4x4(rotate_z, rotate_y);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, rotate_x);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, translate);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, scale);
    }
    else
    {
        model_matrix = Math3d::mat_mult_4x4(translate, rotate_z);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, rotate_y);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, rotate_x);
        model_matrix = Math3d::mat_mult_4x4(model_matrix, scale);
    }
    recalculate_matrix = false;
}



/*update functions*/

void Sprite::update_shader(std::string new_shader)
{
    shader_name = new_shader;
}

void Sprite::update_texture(std::string new_texture)
{
    texture_name = new_texture;
}

void Sprite::update_vbo(std::string new_vbo)
{
    vbo_name = new_vbo;
}

void Sprite::update_int_in(std::map<std::string,int> new_int_in)
{
    int_in = new_int_in;
}

void Sprite::update_float_in(std::map<std::string,float> new_float_in)
{
    float_in = new_float_in;
}

void Sprite::update_color(GLfloat *rgb)
{
    rgbcolor = rgb;
}

void Sprite::update_color(GLfloat r, GLfloat g, GLfloat b)
{
    rgbcolor[0] = r;
    rgbcolor[1] = g;
    rgbcolor[2] = b;
}

void Sprite::gradual_color(GLfloat r, GLfloat g, GLfloat b, GLfloat time_sec)
{
    goal_r = r;
    goal_g = g;
    goal_b = b;

    gradual_dr = (r - rgbcolor[0])/time_sec;
    gradual_dg = (g - rgbcolor[1])/time_sec;
    gradual_db = (b - rgbcolor[2])/time_sec;

    is_gradual_color = true;
}

void Sprite::update_alpha(GLfloat a)
{
    alpha = a;
}

void Sprite::update_rotate_point(GLfloat x, GLfloat y, GLfloat z)
{
    rotate_point_x = x;
    rotate_point_y = y;
    rotate_point_z = z;
}

void Sprite::gradual_fade(GLfloat a, GLfloat time_sec)
{
    goal_a = a;
    gradual_da = (a - alpha)/time_sec;
    is_gradual_fade = true;
}

void Sprite::use_texture(bool b)
{
    has_texture = b;
}

void Sprite::use_rotate_point(bool b)
{
    rotate_around_point = b;
}

/*
    Tranformation functions below. 
    Update means set <x,y,z> as the transform.
    Add means add <dx,dy,dz> to the transform.
    Gradual means gradually change from the current transform to <x,y,z>
        over the time period of time_sec (in seconds)
*/

//translate functions

void Sprite::update_translate(GLfloat x, GLfloat y, GLfloat z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
    recalculate_matrix = true;
}

void Sprite::add_translate(GLfloat dx, GLfloat dy, GLfloat dz)
{
    pos_x += dx;
    pos_y += dy;
    pos_z += dz;
    recalculate_matrix = true;
}

void Sprite::gradual_translate(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec)
{
    goal_pos_x = x;
    goal_pos_y = y;
    goal_pos_z = z;
    gradual_pos_dx = (x-pos_x)/time_sec;
    gradual_pos_dy = (y-pos_y)/time_sec;
    gradual_pos_dz = (z-pos_z)/time_sec;
    
    is_gradual_translate = true;
}

//update position for squares - everything in pixels

void Sprite::update_position(int x, int y, int layer)
{
    GLfloat conv = window_height / 400.f * 50.f;
    pos_z = layer * 0.0001f;
    //2 units is [conv]px - basis of conversion
    GLfloat min_x = (GLfloat) window_width * (-2.0f/conv)/2.0f;    
    GLfloat min_y = (GLfloat) window_height * (2.0f/conv)/2.0f;
    pos_x = min_x + x*(2.0f/conv) + scale_x;
    pos_y = min_y - y*(2.0f/conv) - scale_y;

    this->x = x;
    this->y = y;
    this->layer = layer;

    recalculate_matrix = true;
}

void Sprite::add_position(int dx, int dy)
{
    x += dx;
    y += dy;

    GLfloat conv = window_height / 400.f * 50.f;
    //2 units is [conv]px - basis of conversion
    GLfloat min_x = (GLfloat) window_width * (-2.0f/conv)/2.0f;    
    GLfloat min_y = (GLfloat) window_height * (2.0f/conv)/2.0f;
    pos_x = min_x + x*(2.0f/conv) + scale_x;
    pos_y = min_y - y*(2.0f/conv) - scale_y;

    recalculate_matrix = true;
}

void Sprite::gradual_position(int x, int y, GLfloat time_sec)
{
    goal_x = x;
    goal_y = y;

    gradual_dx = (x - this->x)/time_sec;
    gradual_dy = (y - this->y)/time_sec;

    is_gradual_position = true;

}


//rotation functions

void Sprite::update_rotate(GLfloat x, GLfloat y, GLfloat z)
{
	rot_x = x;
	rot_y = y;
	rot_z = z;
    recalculate_matrix = true;
}

void Sprite::add_rotate(GLfloat dx, GLfloat dy, GLfloat dz)
{
    rot_x += dx;
    rot_y += dy;
    rot_z += dz;
    recalculate_matrix = true;
}

void Sprite::gradual_rotate(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec)
{
    goal_rot_x = x;
    goal_rot_y = y;
    goal_rot_z = z;
    gradual_rot_dx = (x-rot_x)/time_sec;
    gradual_rot_dy = (y-rot_y)/time_sec;
    gradual_rot_dz = (z-rot_z)/time_sec;
    
    is_gradual_rotate = true;
}

//scale functions

void Sprite::update_scale(GLfloat x, GLfloat y, GLfloat z)
{
	scale_x = x;
	scale_y = y;
	scale_z = z;
    recalculate_matrix = true;
}

void Sprite::add_scale(GLfloat dx, GLfloat dy, GLfloat dz)
{
    scale_x += dx;
    scale_y += dy;
    scale_z += dz;
    recalculate_matrix = true;
}

void Sprite::gradual_scale(GLfloat x, GLfloat y, GLfloat z, GLfloat time_sec)
{
    goal_scale_x = x;
    goal_scale_y = y;
    goal_scale_z = z;
    gradual_scale_dx = (x-scale_x)/time_sec;
    gradual_scale_dy = (y-scale_y)/time_sec;
    gradual_scale_dz = (z-scale_z)/time_sec;
    
    is_gradual_scale = true;
}

//scale functions for squares (based on pixels)

void Sprite::update_size(int w, int h)
{
    GLfloat conv = window_height / 400.f * 50.f;
    scale_x = w/conv;
    scale_y = h/conv;

    this->w = w;
    this->h = h;

    recalculate_matrix = true;
}

void Sprite::add_size(int dw, int dh)
{
    w += dw;
    h += dh;

    GLfloat conv = window_height / 400.f * 50.f;
    scale_x = w/conv;
    scale_y = h/conv;

    recalculate_matrix = true;

}

void Sprite::gradual_size(int w, int h, GLfloat time_sec)
{

    goal_w = w;
    goal_h = h;
    gradual_dw = (w - this->w)/time_sec;
    gradual_dh = (h - this->h)/time_sec;

    is_gradual_size = true;
}



/*
    Getters below
*/

std::string Sprite::get_shader_name()
{
    return shader_name;
}

std::string Sprite::get_texture_name()
{
    return texture_name;
}

std::string Sprite::get_vbo_name()
{
    return vbo_name;
}

std::map<std::string, int> Sprite::get_int_in()
{
    return int_in;
}

std::map<std::string, float> Sprite::get_float_in()
{
    return float_in;
}

GLfloat* Sprite::get_color()
{
    return rgbcolor;
}

GLfloat Sprite::get_alpha()
{
    return alpha;
}

void Sprite::update_MM(double curr_time)
{
    if (last_time_drawn < 0) last_time_drawn = curr_time;
    
    if (is_gradual_translate)
    {

        GLfloat test_pos_x=pos_x, test_pos_y=pos_y, test_pos_z=pos_z;
        
        test_pos_x += (gradual_pos_dx * (curr_time - last_time_drawn));
        test_pos_y += (gradual_pos_dy * (curr_time - last_time_drawn));
        test_pos_z += (gradual_pos_dz * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_pos_dx < 0) && (test_pos_x < goal_pos_x) ) || ( (gradual_pos_dx > 0) && (test_pos_x > goal_pos_x) ) ||
            ( (gradual_pos_dy < 0) && (test_pos_y < goal_pos_y) ) || ( (gradual_pos_dy > 0) && (test_pos_y > goal_pos_y) ) ||
            ( (gradual_pos_dz < 0) && (test_pos_z < goal_pos_z) ) || ( (gradual_pos_dz > 0) && (test_pos_z > goal_pos_z) ) )
        {
            //if we have, then set the position as the goal position and stop
	    pos_x = goal_pos_x;
            pos_y = goal_pos_y;
            pos_z = goal_pos_z;
            is_gradual_translate = false;
        }
        else
        {
            pos_x = test_pos_x;
            pos_y = test_pos_y;
            pos_z = test_pos_z;
        }
        recalculate_matrix = true;
    }

    if (is_gradual_rotate)
    {
        GLfloat test_rot_x=rot_x, test_rot_y=rot_y, test_rot_z=rot_z;
        
        test_rot_x += (gradual_rot_dx * (curr_time - last_time_drawn));
        test_rot_y += (gradual_rot_dy * (curr_time - last_time_drawn));
        test_rot_z += (gradual_rot_dz * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_rot_dx < 0) && (test_rot_x < goal_rot_x) ) || ( (gradual_rot_dx > 0) && (test_rot_x > goal_rot_x) ) ||
            ( (gradual_rot_dy < 0) && (test_rot_y < goal_rot_y) ) || ( (gradual_rot_dy > 0) && (test_rot_y > goal_rot_y) ) ||
            ( (gradual_rot_dz < 0) && (test_rot_z < goal_rot_z) ) || ( (gradual_rot_dz > 0) && (test_rot_z > goal_rot_z) ) )
        {
            //if we have, then set the position as the goal position and stop
            rot_x = goal_rot_x;
            rot_y = goal_rot_y;
            rot_z = goal_rot_z;
            is_gradual_rotate = false;
        }
        else
        {
            rot_x = test_rot_x;
            rot_y = test_rot_y;
            rot_z = test_rot_z;
        }
        recalculate_matrix = true;
    }

    if (is_gradual_scale)
    {
        GLfloat test_scale_x=scale_x, test_scale_y=scale_y, test_scale_z=scale_z;
        
        test_scale_x += (gradual_scale_dx * (curr_time - last_time_drawn));
        test_scale_y += (gradual_scale_dy * (curr_time - last_time_drawn));
        test_scale_z += (gradual_scale_dz * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_scale_dx < 0) && (test_scale_x < goal_scale_x) ) || ( (gradual_scale_dx > 0) && (test_scale_x > goal_scale_x) ) ||
            ( (gradual_scale_dy < 0) && (test_scale_y < goal_scale_y) ) || ( (gradual_scale_dy > 0) && (test_scale_y > goal_scale_y) ) ||
            ( (gradual_scale_dz < 0) && (test_scale_z < goal_scale_z) ) || ( (gradual_scale_dz > 0) && (test_scale_z > goal_scale_z) ) )
        {
            //if we have, then set the position as the goal position and stop
            scale_x = goal_scale_x;
            scale_y = goal_scale_y;
            scale_z = goal_scale_z;
            is_gradual_scale = false;
        }
        else
        {
            scale_x = test_scale_x;
            scale_y = test_scale_y;
            scale_z = test_scale_z;
        }
        recalculate_matrix = true;
    }
    if (is_gradual_size)
    {
        GLfloat test_w = w, test_h = w;
        
        test_w += (gradual_dw * (curr_time - last_time_drawn));
        test_h += (gradual_dh * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_dw < 0) && (test_w < goal_w) ) || ( (gradual_dw > 0) && (test_w > goal_w) ) ||
            ( (gradual_dh < 0) && (test_h < goal_h) ) || ( (gradual_dh > 0) && (test_h > goal_h) ) )
        {
            //if we have, then set the position as the goal position and stop
            w = goal_w;
            h = goal_h;
            is_gradual_size = false;
        }
        else
        {
            w = test_w;
            h = test_h;
        }
        GLfloat conv = window_height / 400.f * 50.f;
        scale_x = w/conv;
        scale_y = h/conv;
        recalculate_matrix = true;
    }
    if (is_gradual_position)
    {
        GLfloat test_x = x, test_y = y;
        
        test_x += (gradual_dx * (curr_time - last_time_drawn));
        test_y += (gradual_dy * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_dx < 0) && (test_x < goal_x) ) || ( (gradual_dx > 0) && (test_x > goal_x) ) ||
            ( (gradual_dy < 0) && (test_y < goal_y) ) || ( (gradual_dy > 0) && (test_y > goal_y) ) )
        {
            //if we have, then set the position as the goal position and stop
            x = goal_x;
            y = goal_y;
            is_gradual_size = false;
        }
        else
        {
            x = test_x;
            y = test_y;
        }
        GLfloat conv = window_height / 400.f * 50.f;
        //2 units is [conv]px - basis of conversion
        GLfloat min_x = (GLfloat) window_width * (-2.0f/conv)/2.0f;    
        GLfloat min_y = (GLfloat) window_height * (2.0f/conv)/2.0f;
        pos_x = min_x + x*(2.0f/conv) + scale_x;
        pos_y = min_y - y*(2.0f/conv) - scale_y;
        recalculate_matrix = true;
    }
    if (is_gradual_color)
    {
        GLfloat test_r = rgbcolor[0], test_g = rgbcolor[1], test_b = rgbcolor[2];
        
        test_r += (gradual_dr * (curr_time - last_time_drawn));
        test_g += (gradual_dg * (curr_time - last_time_drawn));
        test_b += (gradual_db * (curr_time - last_time_drawn));

        //test to make sure we haven't overshot
        if( ( (gradual_dr < 0) && (test_r < goal_r) ) || ( (gradual_dr > 0) && (test_r > goal_r) ) ||
            ( (gradual_dg < 0) && (test_g < goal_g) ) || ( (gradual_dg > 0) && (test_g > goal_g) ) ||
            ( (gradual_db < 0) && (test_b < goal_b) ) || ( (gradual_db > 0) && (test_b > goal_b) ) )
        {
            //if we have, then set the position as the goal position and stop
            rgbcolor[0] = goal_r;
            rgbcolor[1] = goal_g;
            rgbcolor[2] = goal_b;
            is_gradual_color = false;
        }
        else
        {
            rgbcolor[0] = test_r;
            rgbcolor[1] = test_g;
            rgbcolor[2] = test_b;
        }
    }
    if (is_gradual_fade)
    {
        GLfloat test_a = alpha;
        test_a += (gradual_da * (curr_time - last_time_drawn));
        if ( ( (gradual_da < 0) && (test_a < goal_a) ) || ( (gradual_da > 0) && (test_a > goal_a) ) )
        {
            alpha = goal_a;
            is_gradual_fade = false;
        }
        else
            alpha = test_a;
    }

    last_time_drawn = curr_time;
    if (recalculate_matrix) calculate_matrix();
}

GLfloat* Sprite::get_MM()
{
    return model_matrix;
}

bool Sprite::is_using_texture()
{
    return has_texture;
}

//convert sprite to container...... i'll probably get rid of this soon
Container Sprite::convert_to_container()
{
    calculate_matrix();
    Container new_container = 
    {
        vbo_name,
        shader_name,
        texture_name,
        model_matrix,
        rgbcolor,
        has_texture
    };

    return new_container;
}
