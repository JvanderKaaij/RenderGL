#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inSurfaceNormals;
layout (location = 2) in vec2 aTextureCoords;

out vec2 TexCoords;
out vec3 ViewDir;

uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 viewPos = view * vec4(aPos, 1.0);

    ViewDir = normalize(viewPos.xyz - vec3(view[3]));
    TexCoords = aTextureCoords;

    vec4 pos = vec4(aPos, 1.0);
    gl_Position = pos;
}