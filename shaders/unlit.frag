#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;
in vec3 DirectionalLight;
in vec2 TextureCoords;

uniform mat4 mvp;
uniform float timer;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1., 0., 0., 1.);
}