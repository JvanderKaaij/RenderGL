#version 450 core

in vec3 SurfaceNormals;
in vec3 WorldNormals;

out vec4 FragColor;

void main()
{
    vec3 ambientColor = vec3(.1, 0., 0.);
    vec3 diffuseColor = vec3(.9, 0., 0.);
    vec3 lightDirection = vec3(0., 0., -1.);

    vec3 viewDirection = vec3(0., 0., -1.);

    vec3 specularColor = vec3(1.);
    float specularExponent = 7.;

    vec3 halfwayVector = normalize(lightDirection + viewDirection);
    float specular = pow(max(dot(WorldNormals, halfwayVector), 0.0), specularExponent);

    float diffuse = max(dot(WorldNormals, lightDirection), 0.0);
    vec3 finalColor = ambientColor + diffuse * diffuseColor + specular * specularColor;

    FragColor = vec4(finalColor, 1.);
}