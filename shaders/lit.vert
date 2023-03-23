#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 model;

layout (std140) uniform SceneUniformBlock {
    float time;
    float deltaTime;
    float padding;
    float morePadding;

    vec4 cameraPosition;
    mat4 cameraView;
    mat4 cameraProjection;
    vec4 sceneLightDirection;
    mat4 sceneLightProjection;
};

out vec3 WorldNormal;
out vec3 Position;
out vec2 TextureCoords;

void main(){
    WorldNormal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));

    TextureCoords = aTextureCoords;

    gl_Position = cameraProjection * cameraView * vec4(Position, 1.0);
}