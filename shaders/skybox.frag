#version 450 core

uniform samplerCube skyboxTexture;

in vec2 TexCoords;
in vec3 viewDir;
out vec4 FragColor;

void main() {
    vec3 reflected = reflect(-viewDir, vec3(0.0, 1.0, 0.0));
    vec4 color = texture(skyboxTexture, reflected);
    FragColor = color;
}
