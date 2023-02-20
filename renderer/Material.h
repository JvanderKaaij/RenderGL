#include "Shaders.h"
#include "Texture.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

struct Material {
    Shaders shaders;

    unsigned int programID;
    unsigned int diffuseID;
    unsigned int specularID;

};

#endif