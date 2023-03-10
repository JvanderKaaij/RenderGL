#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float timer;
uniform vec3 directionalLight;
uniform mat4 lightSpaceMatrix;

out vec3 WorldNormal;
out vec3 LocalNormal;
out vec3 Position;
out vec3 DirectionalLight;
out vec2 TextureCoords;
out vec4 FragPosLightSpace;

void main(){
    WorldNormal = mat3(transpose(inverse(model))) * aNormal;
    LocalNormal = aNormal;
    Position = vec3(model * vec4(aPos, 1.0));

    DirectionalLight = directionalLight;
    TextureCoords = aTextureCoords;

    FragPosLightSpace = lightSpaceMatrix * vec4(Position, 1.0);

    gl_Position = projection * view * vec4(Position, 1.0);
}