#include <vector>
#include "glm/vec3.hpp"


#ifndef RENDERGL_DIRECTIONALLIGHT_H
#define RENDERGL_DIRECTIONALLIGHT_H

struct DirectionalLight {
    glm::vec3 direction = glm::vec3(1,-1,0);
    float intensity = 1.0f;
};

#endif