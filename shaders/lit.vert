#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inSurfaceNormals;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float timer;
uniform vec3 directionalLight;

out vec3 SurfaceNormals;
out vec3 DirectionalLight;
out vec2 TextureCoords;
out vec3 ViewDir;

void main(){
    SurfaceNormals = inSurfaceNormals;
    DirectionalLight = directionalLight;
    TextureCoords = aTextureCoords;

    vec4 viewPos = view * vec4(aPos, 1.0);
    ViewDir = normalize(viewPos.xyz - vec3(view[3]));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}