#version 450 core

in vec4 SurfaceNormals;

out vec4 FragColor;

void main()
{
    FragColor = SurfaceNormals;
}