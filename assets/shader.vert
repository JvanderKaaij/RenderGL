#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inSurfaceNormals;

uniform mat4 mvp;
uniform float timer;
uniform vec3 directionalLight;

out vec3 SurfaceNormals;
out vec3 WorldNormals;
out vec3 DirectionalLight;

void main(){
    gl_Position = mvp * vec4(position, 1.0);
    SurfaceNormals = inSurfaceNormals;
    WorldNormals = normalize(mat3(mvp) * inSurfaceNormals);
    DirectionalLight = directionalLight;
}