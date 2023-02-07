#include "Shaders.h"
#include "Texture.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

class Material {
public:
    Material(Mesh mesh);
    GLuint programID;
    Shaders shaders;
    Texture diffuseTexture;
    Texture specularTexture;
};

#endif