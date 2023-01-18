#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

uniform mat4 mvp;
uniform float timer;

out vec4 Color;

void main(){
    gl_Position = mvp * vec4(position, 1.0);;
    Color = vec4(inColor, 1.0f);
}