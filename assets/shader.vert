#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inSurfaceNormals;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 mvp;
uniform float timer;
uniform vec3 directionalLight;

out vec3 SurfaceNormals;
out vec3 DirectionalLight;
out vec2 TextureCoords;

void main(){
    SurfaceNormals = inSurfaceNormals;
    DirectionalLight = directionalLight;
    TextureCoords = aTextureCoords;
    gl_Position = mvp * vec4(position, 1.0);
}