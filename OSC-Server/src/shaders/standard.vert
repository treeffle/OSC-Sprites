attribute vec3 normal;
attribute vec3 vertex;
attribute vec2 LTexCoord;
varying vec2 texCoord;
varying vec3 var_L;
varying vec3 var_N;

uniform mat4 PM;
uniform mat4 VM;
uniform mat4 MM;


void main()
{

    vec4 V = vec4(vertex, 1.0);
 
    vec4 final_pos =  PM * VM * MM * V;
    var_N = normal;
    var_L = vec3(0.0,0.0,1.0);
    texCoord = LTexCoord;
    gl_Position = final_pos;

}