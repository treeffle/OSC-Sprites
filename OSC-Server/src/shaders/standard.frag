
/*
there's no light yet. and no way of telling whether there's a texture or not.
 so this frag shader is really only good for applying textures
 and coloring everything an ugly grey if there's no texture.
*/


uniform float time;
uniform bool has_texture;
uniform vec4 rgbcolor;
uniform sampler2D diffuse;
uniform float alpha;

varying vec3 var_N;
varying vec3 var_L;
varying vec2 texCoord;

//may have to change alpha thing eventually when dealing with images that have transparency
//but works for now

void main()
{
    vec4 color = rgbcolor;

    vec3 V = vec3(0.0,0.0,1.0);
    vec3 light = vec3(0.5,0.5,0.6);
    vec3 L = normalize(var_L);
    vec3 N = normalize(var_N);
    vec3 H = normalize(L + V);
    
    vec4 D = texture2D(diffuse, texCoord); 
    vec4 S = vec4(0.0,0.0,0.0,1.0);
    float n = 1.0;
    
    float kd = max(dot(N,L), 0.0);
    float ks = pow(max(dot(N,H),0.0), n);

    vec3 rgb;
    float a;

    if(has_texture)    
    {   
        rgb = D.rgb * ks + S.rgb * ks;
        a = D.a;
    }

    else
    {
        rgb = rgbcolor.rgb * ks + S.rgb * ks;
        a = rgbcolor.a;
    }


    gl_FragColor = vec4(rgb,alpha);
    
}