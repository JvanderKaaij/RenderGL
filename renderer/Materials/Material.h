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
    unsigned int diffuseTextID;
    unsigned int specularTextID;
    unsigned int renderedTextID;
    unsigned int cubemapID;
    unsigned int shadowMapID;
    unsigned int sceneBlockUBO;

    glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

};
#endif