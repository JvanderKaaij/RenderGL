#version 450 core

in vec3 Normal;
in vec3 Position;

in vec3 WorldNormal;
in vec3 LocalNormal;
in vec3 DirectionalLight;
in vec2 TextureCoords;
in vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float timer;
uniform vec3 cameraPosition;
uniform vec3 diffuseColor;

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform samplerCube skyboxTexture;
layout(binding = 3) uniform sampler2D shadowMapTexture;
out vec4 FragColor;

float reflectionFactor = 0.2;
float specularExponent = 100.;

vec3 ambient = vec3(0.1);

vec3 viewDirection = vec3(0., 0., -1.);
vec3 specularColor = vec3(1.);

layout (std140) uniform SceneUniformBlock {
    uniform vec4 lightColor;
    uniform mat4 cameraView;
//    uniform vec3 cameraPosition;
    uniform mat4 cameraProjection;
};

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; //should happen in the vertex shader

    float closestDepth = texture(shadowMapTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(LocalNormal, DirectionalLight * 4.0)), 0.005);
    //float bias = 0.01;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

//(This is not following the render equation yet)
//and messed up in all sorts of ways
void main()
{
    vec3 world = normalize(mat3(projection * view * model) * WorldNormal);
    vec3 worldM = normalize(mat3(projection * model) * WorldNormal);
//
    //SPECULAR
    vec3 halfwayVector = normalize(DirectionalLight + viewDirection);
    float specular = pow(max(dot(world, halfwayVector), 0.0), specularExponent);
    float specularIntensity = texture(specularTexture, TextureCoords).r;
    vec3 finalSpecular = (specular * specularIntensity) * specularColor;

    //DIFFUSE
    float diffuse = dot(worldM, DirectionalLight);
    vec4 texelColor = texture(diffuseTexture, TextureCoords);
    vec3 finalDiffuse = diffuse * diffuseColor * texelColor.xyz;

    //REFLECTION
    vec3 I = normalize(Position - cameraPosition * mat3(view));
    vec3 R = reflect(I, normalize(WorldNormal));
    vec3 finalReflection = texture(skyboxTexture, R).rgb * reflectionFactor;

    //SHADOW MAP
    float shadow = ShadowCalculation(FragPosLightSpace);

    //FINAL SUMMING
    vec3 finalColor = ambient + (1.0 - shadow) * (finalSpecular + finalDiffuse + finalReflection);

    FragColor = vec4(finalColor, 1.0);
}