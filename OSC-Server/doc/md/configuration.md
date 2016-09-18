#Configuration

The Configuration class handles all the yaml parsing. There are quite a few functions that existed before I started playing with this, and there are a few that I added that are not in use anymore. I will only go over the two functions that I added and are still used.

The yaml parsing uses yaml-cpp 0.3.0, which can be downloaded/installed from [here](https://code.google.com/p/yaml-cpp/downloads/list). Just make sure it is 0.3.0, as more recent versions will not work!

More in-depth information on parsing yaml using yaml-cpp can be in their tutorial [here](https://code.google.com/p/yaml-cpp/wiki/HowToParseADocument).

###ParseSprites(std::string, std::map<std::string, Sprite\> *spritemap)

tl;dr: This accepts a yaml-format string (probably sent from a client) and updates the sprite map according to the information given in the yaml string.

The yaml string must be formatted like so:

```
todo:
  update: true
  scale:
  - 5.0
  - 5.0
  - 5.0
  rgbcolor:
  - 0.0
  - 0.5
  - 0.0
  - 1.0
  rotate:
  - 0.0
  - 0.0
  - 0.0
  pixel:
  - 200
  - 200
  - 400
  - 400
  - 1
  vbo_name: square
  use_pixels: true
  int_in:
    dummy_int: 4
  has_texture: true
  rotate_around_point: false
  tex_name: cat.png
  shader_name: standard
  time: 1.0
  alpha: 1.0
  name: test_sprite
  float_in:
    dummy_float: 2.4
  translate:
  - 3.0
  - 0.0
  - 0.0
  instruction: update
address: /1/launch
sprite_names:
  test_sprite
  cat_sprite
  square2
```

All fields are optional, except for the following:

* todo:
    * time
    * instruction
    * use_pixels
* address
* sprite_names

Following are descriptions of each field:

####address
The OSC address the message is directed to. We don't actually care about this, but it's necessary for the OSC server. The addresses you can use currently are:

* /1/launch
* /1/transfer
* /1/quit
* /1/exit

####sprite_names
This expects a list of strings. These strings are the name(s) of sprite(s) affected by this message.

####todo
This expects a map of with strings as keys. The values in this map represent actions to be performed on the sprite(s) listed in sprite_names.

####instruction
The instruction specifies how these actions will be carried out. ```instruction: add``` will ensure the add functions of the Sprite class are called. ```instruction:gradual``` will ensure the gradual functions of the Sprite class are called. ```instruction:delete``` will delete the sprite(s). Anything else will make sure the update functions are called.

####time
Expects a float. If using a gradual function, this specifies the duration of the changes. If not using a gradual function, this is still required, but will not affect anything.

####use_pixels
Expects a string ("true" or "false"). If true, it takes into account the pixels field to scale and position the sprite using pixel measurements. If false, it doesn't.

####scale, rotate, translate
These all expect lists of at least three floats and do exactly what you would expect them to. If you're using pixel values, then scale and translate will not be used.

####rgbcolor
This expects a list of three floats, each on a range of [0.0, 1.0], and they represent the rgb color of the sprite.

####alpha
This expects a single float on a range of [0.0, 1.0] that represents the alpha value of the sprite.

####pixels
This expects a list of five integers. The meanings of these five integers are:

* pixels[0]: x-position
* pixels[1]: y-position
* pixels[2]: width
* pixels[3]: height
* pixels[4]: layer

If using pixels, this field scales and positions the sprite such that, if it were a square, it would have these dimensions and location.

####vbo_name, shader_name, tex_name
Each of these expects a string. The vbo_name specifies which vertex/element buffer objects to use for this sprite. The shader_name specifies which shader program to use for this sprite. The tex_name specifies which texture to use for this sprite.

####has_texture
This expects a string ("true" or "false"). If true, it means the sprite has a texture and it is assumed to have the name of that specified in tex_name. If false, it means the sprite does not use a texture and tex_name will be ignored.

####rotate_around_point
This expexts a string ("true" or "false"). If false, when rotated, a sprite will rotate in place. If true, when rotated, a sprite will rotate around the origin.

####int_in, float_in
If you're using your own shader program for this sprite, it may require inputs that SpriteView does not keep track of. int_in and float_in expect maps, both using strings as keys, and int_in having integers as a values and float_in having floats as values. These key/value pairs will be input into the shader program as uniform int/uniform float.

###void ParseTodo(const YAML::Node &, std::vector<std::string>, std::map<std::string, Sprite> *)

ParseTodo is called by ParseSprites and specifically parses the todo node. It applies all actions specified by the todo node to all the sprites listed in sprite_names.

###ParseVertexData(std::string, SpriteView *)

ParseVertexData parses a yaml string containing vertex/element data and has the sprite engine create a vertex buffer object for this data. This allows us to create custom shapes. The incoming data should be of the format

```
name: my_triangle
num_verts: 3
num_elems: 3
vertices:
  - 0.0
  - 0.0
  - 0.0
  - 0.0
  - 1.0
  - 0.0
  - 1.0
  - 0.0
  - 0.0
elements:
  - 0
  - 1
  - 2
```

Vertices are assumed to have three components (x, y, and z) and are automatically divided up when parsing (so the above input has vertices (0.0, 0.0, 0.0), (0.0, 1.0, 0.0), and (1.0, 0.0, 0.0)).

The name given is how you will refer to this shape when creating a sprite. 

Currently this pretty much only works for 2D objects. Because incoming messages are cut off after a certain point, including normal vectors with this data would lower the number of vertices you can include. 