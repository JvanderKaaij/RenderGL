#version 450 core

in vec3 WorldNormal;

uniform vec3 diffuseColor;

out vec4 FragColor;

void main() {

    vec3 worldNormal = normalize(WorldNormal);
    //DIFFUSE
    float diffuse = max(dot(worldNormal, -vec3(0.1, -1.0, 1.0)), 0.0) + 0.3;
    FragColor = vec4(diffuseColor * diffuse, 1.0);
}
