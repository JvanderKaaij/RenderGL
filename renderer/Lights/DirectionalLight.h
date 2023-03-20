#include <vector>
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"


#ifndef RENDERGL_DIRECTIONALLIGHT_H
#define RENDERGL_DIRECTIONALLIGHT_H

class DirectionalLight {
public:
    glm::vec3 direction = glm::vec3(1,-1,0);
    glm::mat4 GetProjectionMatrix();
};

#endif