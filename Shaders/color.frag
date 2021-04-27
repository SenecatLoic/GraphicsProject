#version 140
precision mediump float; //Medium precision for float. highp and smallp can also be used

varying vec3 vary_normal; //Sometimes we use "out" instead of "varying". "out" should be used in later version of GLSL.
varying vec3 vary_position;
varying vec2 vary_uv;
//We still use varying because OpenGLES 2.0 (OpenGL Embedded System, for example for smartphones) does not accept "in" and "out"

uniform vec3 mat_color;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float alpha;

uniform vec3 light_color;
uniform vec3 light_pos;
uniform sampler2D uTexture;
uniform vec3 camera_pos;

void main()
{	
    vec4 color = texture2D(uTexture, vary_uv);

    vec3 norm = vary_normal;
    vec3 lightDir = normalize(light_pos - vary_position); 
    vec3 viewDir = normalize(camera_pos - vary_position );
    vec3 reflectDir = normalize(reflect(-lightDir, norm)); 

    vec3 ambient = ka  * light_color;

    vec3 diffuse = kd * max(0.0,dot(norm, lightDir)) * light_color;

    vec4 diffuseAmbient = vec4(ambient + diffuse,1.0);

    vec3 specular = ks * pow(max(dot(reflectDir,viewDir), 0.0), alpha) * light_color;

    gl_FragColor = diffuseAmbient + vec4(specular, 1.0);
}