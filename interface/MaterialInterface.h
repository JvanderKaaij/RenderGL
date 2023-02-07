#include "../renderer/Shaders.h"
#include "../renderer/Texture.h"

#include <fstream>
#include <assimp/material.h>
#include <iostream>
#include "glad/glad.h"
#include "stb_image.h"

class MaterialInterface {
public:
    static Shaders CompileShaders(std::string vertex_path, std::string fragment_path);
    static Texture LoadTexture(aiTextureType type,aiMaterial* material);
};

