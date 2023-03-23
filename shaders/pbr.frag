#version 450

#define PI 3.1415926538

in vec3 Position;
in vec3 WorldNormal;
in vec2 TextureCoords;
in vec4 FragPosLightSpace;

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

uniform mat4 model;
uniform float roughness;
uniform bool is_metal;
uniform vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(WorldNormal, 1.0);
}
