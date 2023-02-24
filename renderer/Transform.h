
#ifndef RENDERGL_TRANSFORM_H
#define RENDERGL_TRANSFORM_H

#include "glm/vec3.hpp"

struct Transform{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif //RENDERGL_TRANSFORM_H
