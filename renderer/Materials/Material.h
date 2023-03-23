#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Shaders.h"
#include "../Texture.h"
#include "../Transform.h"
#include "../SceneUniformBlock.h"

#ifndef RENDERGL_MATERIAL_H
#define RENDERGL_MATERIAL_H

class Material {
public:
    Material(std::string vertex_path, std::string fragment_path);

    virtual void Draw(Transform transform, SceneUniformBlock* sceneUniforms);

    Shaders shaders;
    unsigned int programID;
    unsigned int renderedTextID;
    unsigned int sceneBlockUBO;
    unsigned int shadowMapID;

};
#endif