#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 mvp;
uniform vec3 tpos;

void main(){
    gl_Position = vec4(tpos, 1.0); //mvp * vec4(position, 1.0);
}