#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 mvp;
uniform float timer;

void main(){
    vec3 pos = position;
    vec4 new_pos =  mvp * vec4(pos, 1.0);
    gl_Position = new_pos;
}