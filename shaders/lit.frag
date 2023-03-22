#version 450 core

in vec3 Normal;
in vec3 Position;

in vec3 WorldNormal;
in vec3 LocalNormal;
in vec3 DirectionalLight;
in vec2 TextureCoords;
in vec4 FragPosLightSpace;

uniform mat4 model;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;

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

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform samplerCube skyboxTexture;
layout(binding = 3) uniform sampler2D shadowMapTexture;

float reflectionFactor = 0.2;
float specularExponent = 100.;

vec3 viewDirection = vec3(0., 0., -1.);
vec3 specularColor = vec3(1.);

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; //should happen in the vertex shader

    float closestDepth = texture(shadowMapTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(LocalNormal, DirectionalLight * 4.0)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

//(This is not following the render equation yet)
//and messed up in all sorts of ways
void main()
{
    vec3 world = normalize(mat3(cameraProjection * cameraView * model) * WorldNormal);
    vec3 worldM = normalize(mat3(cameraProjection * model) * WorldNormal);

    //DIFFUSE
    float diffuse = max(dot(worldM, DirectionalLight), 0.0);
    vec4 texelColor = texture(diffuseTexture, TextureCoords);
    vec3 diffuseAmbient = ambientColor + (diffuse * diffuseColor);
    vec3 finalDiffuse = clamp(diffuseAmbient, vec3(0.0), vec3(1.0)) * texelColor.xyz;

    //SPECULAR
    vec3 halfwayVector = normalize(DirectionalLight + viewDirection);
    float specular = pow(max(dot(world, halfwayVector), 0.0), specularExponent);
    float specularIntensity = texture(specularTexture, TextureCoords).r;
    vec3 finalSpecular = (specular * specularIntensity) * specularColor;

    //REFLECTION
    vec3 I = normalize(Position - cameraPosition.xyz * mat3(cameraView));
    vec3 R = reflect(I, normalize(WorldNormal));
    vec3 finalReflection = texture(skyboxTexture, R).rgb * reflectionFactor;

    //SHADOW MAP
    float shadow = ShadowCalculation(FragPosLightSpace);

    //FINAL SUMMING
    vec3 finalColor = (1.0 - shadow) * (finalSpecular + finalDiffuse + finalReflection);

    FragColor = vec4(finalColor, 1.0);
}