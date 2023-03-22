#ifndef RENDERGL_SCENEUNIFORMBLOCK_H
#define RENDERGL_SCENEUNIFORMBLOCK_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

struct SceneUniformBlock{
    //note that I need the padding to have a 16byte alignment
    float time = 0.0f;
    float deltaTime = 0.0f;
    float padding = 0.0f;
    float morePadding = 0.0f;

    glm::vec4 cameraPosition = glm::vec4(1.0);
    glm::mat4 cameraView = glm::mat4(1.0);
    glm::mat4 cameraProjection = glm::mat4(1.0);

    glm::vec4 sceneLightDirection = glm::vec4(1.0);
    glm::mat4 sceneLightProjection = glm::mat4(1.0);
};

#endif //RENDERGL_SCENEUNIFORMBLOCK_H
