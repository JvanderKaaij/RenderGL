#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;

out vec4 FragColor;

void main()
{
    vec3 ambientColor = vec3(1., 0., 0.);
    vec3 diffuseColor = vec3(1., 1., 1.);
    vec3 lightDirection = vec3(0., 0., -1.);

    float diffuse = max(dot(WorldNormals, lightDirection), 0.0);
    vec3 finalColor = ambientColor + diffuse * diffuseColor;

    FragColor = vec4(finalColor, 1.);
}