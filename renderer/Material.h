#include "Shaders.h"
#include "Texture.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

class Material {
public:
    Material();
    Material(std::string vertex_path, std::string fragment_path);

    virtual void Draw();

    Shaders shaders;
    unsigned int programID;
    unsigned int diffuseID;
    unsigned int specularID;
};
#endif