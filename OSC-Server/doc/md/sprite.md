#Sprite

A sprite acts sorta like a single model. It uses a single vertex buffer object, element buffer object, shader program, and texture.

The Sprite class keeps track of things like the vbo name, shader program name, and texture name, as well as transformation information. 

Functions are offered for modifying these values and doing special calculations

###void update_*(stuff)

Calling any update function simply updates the sprite's value(s) to the given value(s).

###void add_*(stuff)

Calling any add function adds the given values to the sprite's values. In hindsight this isn't so great because if a packet is dropped on one Pi, it goes out of sync.

###void gradual_*(stuff, GLfloat time_sec)

Calling any gradual function gradually updates the sprite's value(s) to the given value(s) over a given period of time (in seconds).