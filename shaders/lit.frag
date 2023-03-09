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

layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform samplerCube skyboxTexture;
out vec4 FragColor;

float reflectionFactor = 0.2;
float specularExponent = 100.;

vec3 diffuseColor = vec3(1.,0., 0.);

vec3 viewDirection = vec3(0., 0., -1.);
vec3 specularColor = vec3(1.);

void main()
{
    vec3 world = normalize(mat3(projection * view * model) * WorldNormal);
//
    //SPECULAR
    vec3 halfwayVector = normalize(DirectionalLight + viewDirection);
    float specular = pow(max(dot(world, halfwayVector), 0.0), specularExponent);
    float specularIntensity = texture(specularTexture, TextureCoords).r;
    vec3 finalSpecular = (specular * specularIntensity) * specularColor;


    //DIFFUSE (took out some part)
    vec4 texelColor = texture(diffuseTexture, TextureCoords);
    vec3 finalDiffuse = diffuseColor * texelColor.xyz;

    //REFLECTION
    vec3 I = normalize(Position - cameraPosition * mat3(view));
    vec3 R = reflect(I, normalize(WorldNormal));

    vec3 finalReflection = texture(skyboxTexture, R).rgb * reflectionFactor;

    //FINAL SUMMING
    vec3 finalColor = finalSpecular + finalDiffuse + finalReflection;

    FragColor = vec4(finalColor, 1.0);
}