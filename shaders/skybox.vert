#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inSurfaceNormals;

out vec3 TexCoords;

void main(){
    TexCoords = position;
    gl_Position = vec4(position, 1.0);
}