#version 450 core

uniform samplerCube skyboxTexture;

in vec3 TexCoords;
in vec3 viewDir;
out vec4 FragColor;

void main() {
    vec3 reflected = reflect(-viewDir, vec3(0.0, 1.0, 0.0));
    vec4 color = texture(skyboxTexture, normalize(reflected));
    FragColor = color;
}
