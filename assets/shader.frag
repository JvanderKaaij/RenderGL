#version 450 core

in vec4 Color;

out vec4 FragColor;

void main()
{
    FragColor = Color; //vec4(1.0, 0.0, 0.0, 1.0);
}