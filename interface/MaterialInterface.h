#include "../renderer/Shaders.h"
#include "../renderer/Texture.h"

#include <fstream>
#include <assimp/material.h>
#include <iostream>
#include "glad/glad.h"
#include "stb_image.h"

#ifndef RENDERGL_MATERIALINTERFACE_H
#define RENDERGL_MATERIALINTERFACE_H

class MaterialInterface {
public:
    static Shaders CompileShaders(std::string vertex_path, std::string fragment_path);
    static Texture* LoadTexture(std::string full_path);
    static Texture* LoadCubeMapTexture(std::string path);
};

#endif
