#include <vector>
#include "glm/vec3.hpp"


#ifndef RENDERGL_DIRECTIONALLIGHT_H
#define RENDERGL_DIRECTIONALLIGHT_H

struct DirectionalLight {
    glm::vec3 direction;
    float intensity;
};

#endif