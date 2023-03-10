#version 450 core

in vec3 Normal;
in vec3 Position;

in vec3 WorldNormal;
in vec3 LocalNormal;
in vec3 DirectionalLight;
in vec2 TextureCoords;

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

vec3 viewDirection = vec3(0., 0., -1.);
vec3 specularColor = vec3(1.);

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
    vec4 shadowMapTxt = texture(shadowMapTexture, TextureCoords);


    //FINAL SUMMING
    vec3 finalColor = finalSpecular + finalDiffuse + finalReflection;


    FragColor = vec4(finalColor, 1.0);
}