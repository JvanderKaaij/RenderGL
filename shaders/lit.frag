#version 450 core

in vec3 Position;
in vec3 WorldNormal;
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
layout(binding = 4) uniform sampler2D normalTexture;

float reflectionFactor = 0.2;
float specularExponent = 100.;

vec3 specularColor = vec3(1.);

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; //should happen in the vertex shader

    float closestDepth = texture(shadowMapTexture, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //There is still a problem with Directional Light being a direction and not a position
    float bias = max(0.05 * (1.0 - dot(WorldNormal, sceneLightDirection.xyz)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

//(This is not following the render equation yet)
void main()
{
    vec3 viewNormal = normalize(mat3(cameraProjection * cameraView * model) * WorldNormal);
    vec3 worldNormal = normalize(WorldNormal);
    vec3 normal = normalize(mat3(cameraProjection * model) * WorldNormal);

    //NORMAL MAP
    vec4 normalColor = texture(normalTexture, TextureCoords);
//    normal *= normalColor.rgb;
//    viewNormal *= normalColor.rgb;
//    worldNormal *= normalColor.rgb;

    //DIFFUSE
    float diffuse = max(dot(worldNormal, -sceneLightDirection.xyz), 0.0);
    vec4 texelColor = texture(diffuseTexture, TextureCoords);
    vec3 diffuseAmbient = ambientColor + (diffuse * diffuseColor);
    vec3 finalDiffuse = clamp(diffuseAmbient, vec3(0.0), vec3(1.0)) * texelColor.xyz;

    //SPECULAR
    vec3 viewDir = normalize(cameraPosition.xyz - Position);
    vec3 reflectDir = reflect(sceneLightDirection.xyz, worldNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 finalSpecular = 0.5 * spec * vec3(1.0);

    //REFLECTION
    vec3 I = normalize(Position + cameraPosition.xyz);
    vec3 R = reflect(I, normalize(worldNormal));
    vec3 finalReflection = texture(skyboxTexture, R).rgb * reflectionFactor;

    //SHADOW MAP
    float shadow = ShadowCalculation(FragPosLightSpace);

    //FINAL SUMMING
    vec3 finalColor = (1.0 - shadow) * (finalSpecular + finalDiffuse) + finalReflection;


    FragColor = texelColor; //vec4(finalColor, 1.0);
}