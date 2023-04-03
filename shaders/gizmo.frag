#version 450 core

in vec3 Normal;

uniform vec3 diffuseColor;

out vec4 FragColor;

void main() {

    vec3 normal = normalize(Normal);
    //DIFFUSE
    float diffuse = max(dot(normal, -vec3(0.1, -1.0, 1.0)), 0.0) + 0.3;
    FragColor = vec4(diffuseColor * diffuse, 1.0);
}
