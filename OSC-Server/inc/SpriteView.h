#ifndef SPRITEVIEW_H_
#define SPRITEVIEW_H_

#include <math.h>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <utility>
#include <assert.h>


#ifdef RPI
 	#include "bcm_host.h"
#endif

#include "image.h"
#include "sprite.h"
#include "verts.h"

//not really used right now
void idle();
void motion(int x, int y);
void mouse(int button, int state, int x, int y);

class SpriteView {

//static functions everywhere
public:
	SpriteView(){};
	SpriteView(int,int,std::map<std::string,Sprite>*);
	virtual ~SpriteView();
	GLfloat rotation_x;
	GLfloat rotation_y;
	GLfloat position_z;
	GLfloat scale;
	int  click_button;
	GLfloat click_rotation_x;
	GLfloat click_rotation_y;
	GLfloat click_scale;                                            
	GLfloat click_nx;
	GLfloat click_ny;

	int wwidth, wheight;

	static long start_time;

	static void display_sprites(void);
	static void display_containers(void);

    #ifdef RPI
	    uint32_t screen_width;
	    uint32_t screen_height;
	    static EGLDisplay egl_display;
	    static EGLSurface egl_surface;
	    EGLContext egl_context;
    #endif

	static GLfloat *PM; //projection matrix
	static GLfloat *VM; //view matrix

	// data members
	static std::map <std::string, Container> *cont_map;
	static std::map <std::string, Sprite> *sprite_map;
	static std::map <std::string, GLuint> shader_map;
	static std::map <std::string, GLuint> texture_map;
	static std::map <std::string, GLuint> vbo_map;
	static std::map <std::string, GLuint> ebo_map;
	static std::map <std::string, int> num_verts_map;
	static std::map <std::string, int> num_elems_map;

	//initializing functions
	void init_engine();
	void init_pm(int, int);
		//for shader programs
	void init_shader_map(std::vector<std::string>);
    GLuint init_shader(GLuint, const char*);
	GLuint init_program(GLuint, GLuint);
	GLchar* load(const char*);
		//for textures
	void init_texture_map(std::vector<std::string>);
	void init_tex(std::string);
		//for vbo/ebos
	void init_vbo_map(std::vector<std::string>);
	void init_standard_vbos();
	void init_custom_vbo(std::string);
	void init_vbo(int, int, vert*, GLushort*, std::string);


	//other important miscellaneous functions
	void start();
	void run();
	void set_vm();
	int get_mouse(int *outx, int *outy);
	static void render(Container cont);
	static void render(Sprite* sprite);
	void exit_func();

	void initTestMap();
};

#endif /* SPRITEVIEW_H_ */
