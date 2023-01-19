#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inSurfaceNormals;

uniform mat4 mvp;
uniform float timer;

out vec4 SurfaceNormals;

void main(){
    gl_Position = mvp * vec4(position, 1.0);;
    SurfaceNormals = vec4(inSurfaceNormals, 1.0f);
}