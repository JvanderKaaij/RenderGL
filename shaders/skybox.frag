#version 450 core

uniform samplerCube skyboxTexture;

in vec3 TexCoords;
out vec4 FragColor;

void main() {
    vec3 direction = normalize(TexCoords);
    vec4 color = texture(skyboxTexture, direction);
    FragColor = color;
}
