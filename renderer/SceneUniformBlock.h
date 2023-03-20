#ifndef RENDERGL_SCENEUNIFORMBLOCK_H
#define RENDERGL_SCENEUNIFORMBLOCK_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

struct SceneUniformBlock{
    glm::vec3 lightColor;

    glm::vec3 cameraPosition = glm::vec3(1.0);
    glm::mat4 cameraView = glm::mat4(1.0);
    glm::mat4 cameraProjection = glm::mat4(1.0);
};

#endif //RENDERGL_SCENEUNIFORMBLOCK_H
