#include "../Shaders.h"
#include "../Texture.h"
#include "../Transform.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

class Material {
public:
    Material(std::string vertex_path, std::string fragment_path);

    void virtual Draw(Transform transform);

    Shaders shaders;
    unsigned int programID;
    unsigned int diffuseID;
    unsigned int specularID;
    unsigned int renderedTextureID;
    
};
#endif