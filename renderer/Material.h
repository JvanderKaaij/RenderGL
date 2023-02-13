#include "Shaders.h"
#include "Texture.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

struct Material {
    Shaders shaders;
    Texture diffuseTexture;
    Texture specularTexture;
};

#endif