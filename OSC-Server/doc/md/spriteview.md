#SpriteView

The SpriteView class is in charge of initializing the OpenGL ES context on the Raspberry Pi, as well as initializing all textures, buffer objects, and shaders. It's also in charge of rendering all sprites in sprite_map.

All initialization of textures, buffer objects, and shaders happens during the context initialization, and does not happen on the fly, so make sure these are all available on itialization.

The three key functions needed for incorporating this into other projects are:

* start() - called once to initialize the context and engine
* display_sprites() - called each frame to render sprites in sprite_map
* exit_func() - called once when you want to destroy context

Sprites are added to the sprite map and modified by other parts of your program.

###SpriteView(int, int, std::map<std::string, Sprite) *)

This initializer accepts three inputs:
The resolution of the entire display system (width and height) and a pointer for the Sprite map. 



###void start()

This is called once, when initializing the engine. It uses some Pi magic to initialize an OpenGL ES context, and then calls *init_engine()* to initialize all other stuff.

###void init_engine()

This is called once, at the end of *start()*. Right now things are unfortunately pretty hard-coded.

Three vectors are created and filled with names of shaders, textures, and vertex buffer objects. 

Shader names refer to the names of files containing shaders, minus the file extension (eg. "standard" to use "standard.frag").

Texture names refer to the names of the files containing textures, including the file extension (eg. "cat.png").

Vertex buffer object would ideally refer to the names of files containing vertex information for a model (eg. "cat.obj"), although loading models from files is not supported right now.

After these vectors are created and filled with names, we pass them to three functions: init_shader_map, init_texture_map, and init_vbo_map.

This function also calls functions to initialize the projection matrix, view matrix, and set the viewport. 

###void init_shader_map(std::vector<std::string>)

This is called to initialize all shader programs needed. 

Currently a single vertex shader is used for all programs called *standard.vert*. This vertex shader as inputs uniforms projection matrix, view matrix, and model matrix, and attributes vertex, normal, and LTexCoord. It simply does the calculations to produce the final vertex position, and passes the normal, a dummy light vector, and the texture coordinate to the fragment shader.

The fragment shaders are all user-defined. Provided is a standard shader that either colors the sprite its given color, or if it chooses to use a texture, applies the texture.

This function goes throught the input list of shader names and initializes these as individual shader programs, which are then placed into *shader_map*, using the name as the key.

###void init_texture_map(std::vector<std::string>)

This is called to initialize all textures needed.

It iterates through the list of input texture names and initializes each texture. It then places that texture into *texture_map*, using the name as the key.

###void init_vbo_map(std::vector<std::string>)

This is called to initialize all vertex/element buffer objects needed.

The first thing this function does is call init_standard_vbos(), which initializes the vertex/element buffer objects for a "square", an "isotri" (isosceles triangle), and a "cube".

Then it iterates through the list of names and calls init_custom_vbo for each. As of writing, init_custon_vbo is unimplemented.

All vertex buffer objects are stored in *vbo_map*, element buffer objects in *ebo_map*, the number of vertices in *num_verts_map*, and the number of elements in *num_elems_map* - all using the same name as the key.

###void display_sprites()

This is what should be called every frame to render sprites. It iterates through *sprite_map* and calls the render function on each.

###void render(Sprite)

This renders each sprite according to the information stored in the input Sprite and the information in all our local maps.

First it loads the appropriate shader program. Then it updates the time. It asks the sprite to update its model matrix. Then it sends a bunch of information to the shader program: whether it has a texture, what its color is, what the time elapsed is, what the alpha is, and any user-defined input integers and input floats. It then also sends all the matrix and vertex information, as well as the texture. Finally it draws the sprite.

###void exit_func()

This is called on exiting the sprite engine. It destroys the context and releases resources.
