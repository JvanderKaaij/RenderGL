#version 450 core

uniform samplerCube skyboxTexture;

in vec2 TexCoords;
in vec3 ViewDir;
out vec4 FragColor;

void main() {
    vec3 reflected = reflect(-ViewDir, vec3(0.0, 1.0, 0.0));
    vec3 norm_reflected = normalize(reflected);
    vec4 color = texture(skyboxTexture, norm_reflected);
    FragColor = color;
}
