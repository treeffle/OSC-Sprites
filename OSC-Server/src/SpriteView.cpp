#include "SpriteView.h"
#include "math3d.h"
#include <sys/time.h>
#define PI 3.14159265



#ifdef RPI
  EGLDisplay SpriteView::egl_display;
  EGLSurface SpriteView::egl_surface;
#endif

GLfloat *SpriteView::PM;
GLfloat *SpriteView::VM;
long SpriteView::start_time;

std::map <std::string, Container> *SpriteView::cont_map;
//sprite map no longer needed - only for debugging
std::map <std::string, Sprite> *SpriteView::sprite_map;
std::map <std::string, GLuint> SpriteView::shader_map;
std::map <std::string, GLuint> SpriteView::texture_map;
std::map <std::string, GLuint> SpriteView::vbo_map;
std::map <std::string, GLuint> SpriteView::ebo_map;
std::map <std::string, int>  SpriteView::num_verts_map;
std::map <std::string, int>  SpriteView::num_elems_map;


void SpriteView::initTestMap()
{
  (*sprite_map)["test"].update_vbo("square");
  (*sprite_map)["test"].update_texture("none");
  (*sprite_map)["test"].update_color(1.f, 0.f, 1.f);

}

void SpriteView::display_sprites(void)
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(std::map<std::string, Sprite>::iterator map_iter = SpriteView::sprite_map->begin(); map_iter != SpriteView::sprite_map->end(); map_iter++)
  {
    Sprite* current_sprite = &(*map_iter).second;
    SpriteView::render(current_sprite);
  }

  #ifdef RPI
    eglSwapBuffers(SpriteView::egl_display, SpriteView::egl_surface);
  #else
    glutSwapBuffers();
  #endif
}
 

/**
*
SpriteView Zone
*
**/

SpriteView::SpriteView(int window_width, int window_height, std::map<std::string, Sprite>* new_map) {

  wwidth = window_width;
  wheight = window_height;
  SpriteView::sprite_map = new_map;

}

SpriteView::~SpriteView() {
  // TODO Auto-generated destructor stub
}




/*---------initializing functions---------*/

/*
  initializing the engine
  takes the window width and height and a pointer to a map of containers
*/

void SpriteView::init_engine()
{
  timeval tv;
  gettimeofday(&tv, NULL); 
  start_time = tv.tv_usec + 1000000 * tv.tv_sec;

  std::cout<<"initializing engine"<<std::endl;
  //SpriteView::cont_map = the_map;
  rotation_x = 0.0;
  rotation_y = 0.0;
  position_z = 0.0;
  scale      = 1.0;
  std::vector<std::string> vbo_names;
  std::vector<std::string> shader_names;
  std::vector<std::string> tex_names;

  shader_names.push_back("standard");
  tex_names.push_back("cat1.png");
  tex_names.push_back("cat2.png");

  // TODO: Have viewport dimensions/positions be initialized from config
  glViewport(300,0,wwidth,wheight); //for left display
  //glViewport(-(768+768/15.4*1.235*2),0,wwidth,wheight); //for middle display
  //glViewport(-2*(768+768/15.4*1.235*2),0,wwidth,wheight); //for right display
  //init projection matrix
  init_pm(wwidth,wheight);
  set_vm();
  
  //init our maps
  init_shader_map(shader_names);
  init_texture_map(tex_names);
  init_vbo_map(vbo_names);

  
  //enablers
  glEnable(GL_DEPTH_TEST);
  //necessary for transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


/*
  initializes the projection matrix
  takes the screen width and height
*/
void SpriteView::init_pm(int w, int h)
{
  float n = 1.0;         //near cutoff
  float f = 100.0;    //far cutoff
  float t =  1.0;
  float b = -1.0;
  float l = -(float)w/(float)h;
  float r = (float)w/(float)h;
  
  SpriteView::PM = new GLfloat[16];
  for (int i = 0; i < 16; i++) SpriteView::PM[i] = 0.f;
  SpriteView::PM[0] = 2*n/(r-l);
  SpriteView::PM[5] = 2*n/(t-b);
  SpriteView::PM[8] = (r+l)/(r-l);
  SpriteView::PM[9] = (t+b)/(t-b);
  SpriteView::PM[10] = -(f+n)/(f-n);
  SpriteView::PM[11] = -1.f;
  SpriteView::PM[14] = -2*f*n/(f-n);
}


/*---------initializing shader programs---------*/

/*
  initializes the shader map
  takes a list of shader names - appends .frag to the name to find the file

  standard.vert is always the used as the vertex shader
*/
void SpriteView::init_shader_map(std::vector<std::string> shader_list)
{
  //for all shaders
  //assume we have a list of std::string shader_name
  
  for(std::vector<std::string>::iterator vec_iter = shader_list.begin(); vec_iter != shader_list.end(); vec_iter++)
  {
    std::string shader_name = *vec_iter;
    GLuint vert_shader, frag_shader;
    std::string frag_string;
    GLchar * frag_char;

    frag_string = shader_name;
    frag_string.append(".frag");
    frag_char = new char[frag_string.length()+1];
    std::strcpy(frag_char, frag_string.c_str());

    vert_shader = init_shader(GL_VERTEX_SHADER, load("standard.vert"));
    frag_shader = init_shader(GL_FRAGMENT_SHADER, load(frag_char));
    SpriteView::shader_map[shader_name] = init_program(vert_shader,frag_shader);
  }
  
}


/*
  initializes a vertex shader or frag shader
  takes the shader type (either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
    and a GLchar array with the contents of the shader
  returns the vert or frag shader
*/
GLuint SpriteView::init_shader(GLuint shader_type, const char *text)
{
  GLuint shader;
  shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, (const GLchar **) &text, 0);
  glCompileShader(shader);

  //error check
  GLchar *p;
  GLint status, length;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  if ((status == 0) && (p = (GLchar *) calloc(length+1,1)))
    {
      glGetShaderInfoLog(shader,length,NULL,p);
      fprintf(stderr, "OpenGL Shader Error:\n%s",p);
      free(p);
    }
  return shader;
}


/*
  initializes a shader program
  takes a vertex shader and a frag shader
  returns a shader program
*/
GLuint SpriteView::init_program(GLuint vert_shader, GLuint frag_shader)
{
  GLuint program;
  program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);

  //link error check
  GLchar *p;
  GLint status, length;

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  if ((status == 0) && (p = (GLchar *) calloc(length+1,1)))
  {
    glGetProgramInfoLog(program, length, NULL, p);
    std::cout << "OpenGL Program Error:\n"<<p;
    free(p);
  }
  return program;
}


/*
  loads a file and sticks it into a GLchar array 
*/
GLchar * SpriteView::load(const char *name)
{
  FILE *fp = 0;
  void *p = 0;
  size_t n = 0;
  
  if ((fp = fopen(name, "rb")))
  {
    if (fseek(fp, 0, SEEK_END) == 0)
      if ((n = (size_t) ftell(fp)))
        if (fseek(fp, 0, SEEK_SET) == 0)
          if ((p = calloc(n+1, 1)))
            fread(p, 1, n, fp);
                      
    fclose(fp);
  }
  return (char *) p;
}


/*---------initializing textures---------*/

/*
  initializes the texture map
  takes a list of texture names - assumes the file extension is in the name
*/
void SpriteView::init_texture_map(std::vector<std::string> tex_list)
{
  for(std::vector<std::string>::iterator vec_iter = tex_list.begin(); vec_iter != tex_list.end(); vec_iter++)
  {
    std::string filename = *vec_iter;
    //for all textures
    init_tex(filename);
  }
}


/*
  initializes a texture using the given filename
  places it in the texture map at the index "<filename>"
*/
void SpriteView::init_tex(std::string filename)
{
  glGenTextures(1, &SpriteView::texture_map[filename]);
  void *p;
  int w, h, c, b;

  char *filename_char = new char[filename.length()+1];
  std::strcpy(filename_char, filename.c_str());

  if ((p = image_read(filename_char, &w, &h, &c, &b)))
  {
    int e = image_external_form(c);
    int t = image_external_type(b);

    glBindTexture(GL_TEXTURE_2D, SpriteView::texture_map[filename]);
    glTexImage2D(GL_TEXTURE_2D, 0, e, w, h, 0, e, t, p);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
}


/*---------initializing vbos/ebos---------*/

/*
  initializes the vbo map
  takes a list of custom vbo names - assumes the name is the filename with extension
  whether or not any custom vbo's are given,
  this will still initialize 3 standard vbos:
    "square" - a unit square
    "cube" - a unit cube
    "isotri" - an iscosoles triangle
*/
void SpriteView::init_vbo_map(std::vector<std::string> vbo_list)
{
  init_standard_vbos();
  //for all custom vbos
  for(std::vector<std::string>::iterator vec_iter = vbo_list.begin(); vec_iter != vbo_list.end(); vec_iter++)
  {
    std::string vbo_name = *vec_iter;
    init_custom_vbo(vbo_name);
  }
}


/*
  initializes the standard vbos
*/
void SpriteView::init_standard_vbos()
{
  init_vbo(4, 6, square_verts, square_elems, "square");
  init_vbo(3, 3, isotri_verts, isotri_elems, "isotri");
  init_vbo(24, 36, cube_verts, cube_elems, "cube");
}


/*
  initializes a custom vbo
  takes a filename and parses through the file to get vertex/normal/index/texture/etc. data
*/
void SpriteView::init_custom_vbo(std::string vbo_name)
{
  //TODO
}


/*
  creates a vbo given the number of vertices and elements and a vertex array and element array  
*/
void SpriteView::init_vbo(int num_verts, int num_elems, vert *verts, GLushort *elems, std::string name)
{
  SpriteView::num_verts_map[name] = num_verts;
  SpriteView::num_elems_map[name] = num_elems;

  glGenBuffers(1,&SpriteView::vbo_map[name]);
  glGenBuffers(1,&SpriteView::ebo_map[name]);

  glBindBuffer(GL_ARRAY_BUFFER, SpriteView::vbo_map[name]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert)*num_verts, verts, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteView::ebo_map[name]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*num_elems, elems, GL_STATIC_DRAW);
}




/*---------other important miscellaneous functions---------*/


/*start function*/
void SpriteView::start()
{
  //rapsberry pi initialization
  #ifdef RPI
    bcm_host_init();

    uint32_t success = 0;
    EGLBoolean result;
    EGLint num_config;
    
    static EGL_DISPMANX_WINDOW_T nativewindow;
    
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
    
    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    //this is the magic that will let me use opengl es 2
    //mysterious errors if removed
    static const EGLint context_attributes[] = 
    {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
    };
   
    EGLConfig config;
    
    //get an EGL display connection
    SpriteView::egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(SpriteView::egl_display != EGL_NO_DISPLAY);
    
    //initialize the EGL display connection
    result = eglInitialize(SpriteView::egl_display, NULL, NULL);
    assert(EGL_FALSE != result);
    
    //get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(SpriteView::egl_display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);
    
   // get an appropriate EGL frame buffer configuration
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);

    //create an EGL rendering context - use the magic
    egl_context = eglCreateContext(SpriteView::egl_display, config, EGL_NO_CONTEXT, context_attributes);
    assert(egl_context != EGL_NO_CONTEXT);
    
    //create an EGL window surface
    success = graphics_get_display_size(0, &screen_width, &screen_height);
    assert(success >= 0);
    
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = screen_width;
    dst_rect.height = screen_height;
    
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screen_width << 16;
    src_rect.height = screen_height << 16;
   
    VC_DISPMANX_ALPHA_T alpha;
    alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
    alpha.opacity = 255;
    alpha.mask = 0;
 
    dispman_display = vc_dispmanx_display_open(0);
    dispman_update = vc_dispmanx_update_start(0);
    
    dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display, 0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, &alpha,0,(DISPMANX_TRANSFORM_T)0);
    
    nativewindow.element = dispman_element;
    nativewindow.width = screen_width;
    nativewindow.height = screen_height;
    vc_dispmanx_update_submit_sync(dispman_update);
    
    SpriteView::egl_surface = eglCreateWindowSurface(SpriteView::egl_display, config, &nativewindow, NULL);
    assert(SpriteView::egl_surface != EGL_NO_SURFACE);
    
    //connect the context to the surface
    result = eglMakeCurrent(SpriteView::egl_display, SpriteView::egl_surface, SpriteView::egl_surface, egl_context);
    assert (EGL_FALSE != result);
  
    init_engine();

    //non raspberry pi startup
  #else
    std::cout << "Initializing GLUT" << std::endl;
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    std::cout << "  initializing window size" << std::endl;
    glutInitWindowSize(500,wheight);
    std::cout << "glutinit"<<std::endl;
    char **argv;
    argv = new char*[1];
    argv[0] = new char[3];
    argv[0][0] = 'y';
    argv[0][1] = 'e';
    argv[0][2] = 'p';
    int one = 1;
    glutInit(&one,argv);

    std::cout << "  creating window" << std::endl;
    glutCreateWindow("Sprites");

    std::cout << " setting callbacks" << std::endl;
    initTestMap();
  //glutReshapeFunc(reshape);
    glutDisplayFunc(SpriteView::display_sprites);
    //glutMotionFunc(motion);
    //glutMouseFunc(mouse);
    //glutIdleFunc(idle);
    std::cout << "GLEW init" << std::endl;

    if (glewInit() == GLEW_OK)
    {
    std::cout << glGetString(GL_VERSION) << std::endl;

    //so here's a test map
    SpriteView::init_engine();
    glutMainLoop();
    } 
  #endif
}


/*run function*/
void SpriteView::run()
{

  #ifdef RPI
    SpriteView::display_sprites();
    //exit_func();
  #else
    
  #endif
}


/*
  sets the view matrix
*/
void SpriteView::set_vm()
{
  GLfloat *t_mat, *rx_mat, *ry_mat, *s_mat;
  
  SpriteView::VM = new GLfloat[16];
  t_mat = new GLfloat[16];
  rx_mat = new GLfloat[16];
  ry_mat = new GLfloat[16];
  s_mat = new GLfloat[16];
  
  //set everything as identity
  for (int i = 0; i < 16; i++)
  {
    if(i % 5 == 0) SpriteView::VM[i] = t_mat[i] = rx_mat[i] = ry_mat[i] = s_mat[i] = 1.f;
      else SpriteView::VM[i] = t_mat[i] = rx_mat[i] = ry_mat[i] = s_mat[i] = 0.f;
  }
    
  //set up transform matrix:
  t_mat[12] = t_mat[13] = 0.f;
  t_mat[14] = -position_z;

  //set up rotation matrices:
  rx_mat[5] = rx_mat[10] = cos(rotation_x * PI/180.0);
  rx_mat[6] = sin(rotation_x * PI/180.0);
  rx_mat[9] = -rx_mat[6];
  
  ry_mat[0] = ry_mat[10] = cos(rotation_y * PI/180.0);
  ry_mat[8] = sin(rotation_y * PI/180.0);
  ry_mat[2] = -ry_mat[8];

  //set up scale matrix:
  s_mat[0] = s_mat[5] = s_mat[10] = scale;

  //matrix multiplication
  SpriteView::VM = Math3d::mat_mult_4x4(t_mat, rx_mat);
  SpriteView::VM = Math3d::mat_mult_4x4(SpriteView::VM, ry_mat);
  SpriteView::VM = Math3d::mat_mult_4x4(SpriteView::VM, s_mat);
}
   


void SpriteView::render(Container cont)
{
  GLuint program = SpriteView::shader_map[cont.shader_name];
  glUseProgram(program);

  glUniform1i(glGetUniformLocation(program,"has_texture"),cont.has_texture);
  glUniform4f(glGetUniformLocation(program,"rgbcolor"),cont.rgbcolor[0],cont.rgbcolor[1],cont.rgbcolor[2],cont.rgbcolor[3]);
  //glUniform1f(glGetUniformLocation(program, "time"), time_elapsed);

  for(std::map<std::string, int>::iterator map_iter = cont.int_in.begin(); map_iter != cont.int_in.end(); map_iter++)
  {
    std::string int_name = map_iter->first;
    int int_val = map_iter->second;  
    char * name_char = new char[int_name.length()+1];
    std::strcpy(name_char, int_name.c_str());  
    glUniform1i(glGetUniformLocation(program,name_char),int_val);
    std::cout<<"int in: "<<name_char<<" = "<<int_val<<std::endl;
  }

  for(std::map<std::string, float>::iterator map_iter = cont.float_in.begin(); map_iter != cont.float_in.end(); map_iter++)
  {
    std::string float_name = map_iter->first;
    float float_val = map_iter->second;    
    char * name_char = new char[float_name.length()+1];
    std::strcpy(name_char, float_name.c_str());  
    glUniform1f(glGetUniformLocation(program,name_char),float_val);
    std::cout<<"float in: "<<name_char<<" = "<<float_val<<std::endl;  
  }

  glUniformMatrix4fv(glGetUniformLocation(program, "PM"), 1, GL_FALSE, SpriteView::PM);
  glUniformMatrix4fv(glGetUniformLocation(program, "VM"), 1, GL_FALSE, SpriteView::VM);
  glUniformMatrix4fv(glGetUniformLocation(program, "MM"), 1, GL_FALSE, cont.transform_matrix);
  const size_t sz = sizeof (GLfloat);
  
  glEnable(GL_TEXTURE_2D);
  if(cont.has_texture) glBindTexture(GL_TEXTURE_2D, SpriteView::texture_map[cont.tex_name]);
  else glBindTexture(GL_TEXTURE_2D, 0);
  glUniform1f(glGetUniformLocation(program, "diffuse"), 0); 
  

  glBindBuffer(GL_ARRAY_BUFFER, SpriteView::vbo_map[cont.vbo_name]);
  glVertexAttribPointer(glGetAttribLocation(program, "vertex"),3, GL_FLOAT, GL_FALSE, sizeof(struct vert), 0);
  glVertexAttribPointer(glGetAttribLocation(program, "normal"),3, GL_FLOAT, GL_FALSE, sizeof(struct vert), (const GLvoid *) (sz * 3));
  glVertexAttribPointer(glGetAttribLocation(program, "LTexCoord"),2,GL_FLOAT, GL_FALSE, sizeof(struct vert), (const GLvoid *) (sz * 6));
  glEnableVertexAttribArray(glGetAttribLocation(program, "vertex"));
  glEnableVertexAttribArray(glGetAttribLocation(program, "normal"));
  glEnableVertexAttribArray(glGetAttribLocation(program, "LTexCoord"));

  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteView::ebo_map[cont.vbo_name]);
  glDrawElements(GL_TRIANGLES, SpriteView::num_elems_map[cont.vbo_name], GL_UNSIGNED_SHORT, 0);
} 

void SpriteView::render(Sprite* sprite)
{
  GLuint program = SpriteView::shader_map[sprite->get_shader_name()];
  glUseProgram(program);

  timeval tv;
  gettimeofday(&tv,NULL);
  long curr_time = tv.tv_usec + 1000000 * tv.tv_sec;
  long diff = curr_time - start_time;  
  double time_elapsed = diff / 1000000.0;
  
  sprite->update_MM(time_elapsed);

  glUniform1i(glGetUniformLocation(program,"has_texture"),sprite->is_using_texture());
  glUniform4fv(glGetUniformLocation(program,"rgbcolor"),4,sprite->get_color());
  glUniform1f(glGetUniformLocation(program, "time"), time_elapsed);
  glUniform1f(glGetUniformLocation(program, "alpha"), sprite->get_alpha());


  std::map<std::string, int> int_in = sprite->get_int_in();
  std::map<std::string, float> float_in = sprite->get_float_in();

  for(std::map<std::string, int>::iterator map_iter = int_in.begin(); map_iter != int_in.end(); map_iter++)
  {
    std::string int_name = map_iter->first;
    int int_val = map_iter->second;  
    char * name_char = new char[int_name.length()+1];
    std::strcpy(name_char, int_name.c_str());  
    glUniform1i(glGetUniformLocation(program,name_char),int_val);
   // std::cout<<"int in: "<<name_char<<" = "<<int_val<<std::endl;
  }

  for(std::map<std::string, float>::iterator map_iter = float_in.begin(); map_iter != float_in.end(); map_iter++)
  {
    std::string float_name = map_iter->first;
    float float_val = map_iter->second;    
    char * name_char = new char[float_name.length()+1];
    std::strcpy(name_char, float_name.c_str());  
    glUniform1f(glGetUniformLocation(program,name_char),float_val);
    //std::cout<<"float in: "<<name_char<<" = "<<float_val<<std::endl;  
  }

  glUniformMatrix4fv(glGetUniformLocation(program, "PM"), 1, GL_FALSE, SpriteView::PM);
  glUniformMatrix4fv(glGetUniformLocation(program, "VM"), 1, GL_FALSE, SpriteView::VM);
  glUniformMatrix4fv(glGetUniformLocation(program, "MM"), 1, GL_FALSE, sprite->get_MM());
  const size_t sz = sizeof (GLfloat);
  
  glEnable(GL_TEXTURE_2D);
  if(sprite->is_using_texture()) glBindTexture(GL_TEXTURE_2D, SpriteView::texture_map[sprite->get_texture_name()]);
  else glBindTexture(GL_TEXTURE_2D, 0);
  glUniform1f(glGetUniformLocation(program, "diffuse"), 0); 
  

  glBindBuffer(GL_ARRAY_BUFFER, SpriteView::vbo_map[sprite->get_vbo_name()]);
  glVertexAttribPointer(glGetAttribLocation(program, "vertex"),3, GL_FLOAT, GL_FALSE, sizeof(struct vert), 0);
  glVertexAttribPointer(glGetAttribLocation(program, "normal"),3, GL_FLOAT, GL_FALSE, sizeof(struct vert), (const GLvoid *) (sz * 3));
  glVertexAttribPointer(glGetAttribLocation(program, "LTexCoord"),2,GL_FLOAT, GL_FALSE, sizeof(struct vert), (const GLvoid *) (sz * 6));
  glEnableVertexAttribArray(glGetAttribLocation(program, "vertex"));
  glEnableVertexAttribArray(glGetAttribLocation(program, "normal"));
  glEnableVertexAttribArray(glGetAttribLocation(program, "LTexCoord"));

  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteView::ebo_map[sprite->get_vbo_name()]);
  glDrawElements(GL_TRIANGLES, SpriteView::num_elems_map[sprite->get_vbo_name()], GL_UNSIGNED_SHORT, 0);
} 


void SpriteView::exit_func()
// Function to be passed to atexit().
{
  std::cout<<"Exiting display"<<std::endl;
  #ifdef RPI
   // clear screen
   glClear( GL_COLOR_BUFFER_BIT );

   //release all buffers
  for(std::map<std::string, GLuint>::iterator map_iter = SpriteView::vbo_map.begin(); map_iter != SpriteView::vbo_map.end(); map_iter++)
  {
    glDeleteBuffers(1, &(*map_iter).second);
  }

  for(std::map<std::string, GLuint>::iterator map_iter = SpriteView::ebo_map.begin(); map_iter != SpriteView::ebo_map.end(); map_iter++)
  {
    glDeleteBuffers(1, &(*map_iter).second);
  }

   eglSwapBuffers(SpriteView::egl_display, SpriteView::egl_surface);

   // Release OpenGL resources
   eglMakeCurrent( SpriteView::egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
   eglDestroySurface( SpriteView::egl_display, SpriteView::egl_surface );
   eglDestroyContext( SpriteView::egl_display, egl_context );
   eglTerminate( SpriteView::egl_display );
  #endif

   //I should probably delete all my textures/buffers/programs too

} // exit_func()


/*----------------------------------------------------------------------------*/







