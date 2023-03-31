#version 450 core
uniform vec3 diffuseColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(diffuseColor, 1.0);
}
