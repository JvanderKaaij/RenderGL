#include <vector>
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"
#include "../Transform.h"


#ifndef RENDERGL_DIRECTIONALLIGHT_H
#define RENDERGL_DIRECTIONALLIGHT_H

class DirectionalLight {
public:
    Transform transform;
    glm::mat4 GetProjectionMatrix();
};

#endif