#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;
in vec3 DirectionalLight;
in vec2 TextureCoords;

uniform mat4 mvp;
uniform float timer;
layout(binding = 0) uniform sampler2D diffuseTexture;
layout(binding = 1) uniform sampler2D specularTexture;

out vec4 FragColor;

void main()
{
    vec3 world = normalize(mat3(mvp) * SurfaceNormals);

    vec3 ambientColor = vec3(.1, 0., 0.);
    vec3 diffuseColor = vec3(1.,1., 1.);

    vec3 viewDirection = vec3(0., 0., -1.);

    vec3 specularColor = vec3(1.);
    float specularExponent = 100.;

    vec3 halfwayVector = normalize(DirectionalLight + viewDirection);
    float specular = pow(max(dot(world, halfwayVector), 0.0), specularExponent);

    float specularIntensity = texture(specularTexture, TextureCoords).r;

    vec3 finalSpecular = (specular * specularIntensity) * specularColor;

    float diffuse = max(dot(world, DirectionalLight), 0.0);
    vec4 texelColor = texture(diffuseTexture, TextureCoords);
    vec3 finalDiffuse = ambientColor + diffuse * diffuseColor * texelColor.xyz;

    vec3 finalColor = (finalDiffuse + finalSpecular);

    FragColor = vec4(finalColor, 1.);
}