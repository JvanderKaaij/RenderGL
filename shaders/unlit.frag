#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;
in vec2 TextureCoords;

layout(binding = 0) uniform sampler2D renderTexture;

out vec4 FragColor;

void main()
{
    vec4 txtrColor = texture(renderTexture, TextureCoords);
    FragColor = vec4(txtrColor);
}