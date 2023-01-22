#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;
in vec3 DirectionalLight;

out vec4 FragColor;

void main()
{
    vec3 ambientColor = vec3(.1, 0., 0.);
    vec3 diffuseColor = vec3(.9, 0., 0.);

    vec3 viewDirection = vec3(0., 0., -1.);

    vec3 specularColor = vec3(1.);
    float specularExponent = 100.;

    vec3 halfwayVector = normalize(DirectionalLight + viewDirection);
    float specular = pow(max(dot(WorldNormals, halfwayVector), 0.0), specularExponent);

    float diffuse = max(dot(WorldNormals, DirectionalLight), 0.0);
    vec3 finalColor = ambientColor + diffuse * diffuseColor + specular * specularColor;

    FragColor = vec4(finalColor, 1.);
}