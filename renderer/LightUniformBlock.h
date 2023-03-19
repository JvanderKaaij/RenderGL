//
// Created by Joey on 20/03/2023.
//

#ifndef RENDERGL_LIGHTUNIFORMBLOCK_H
#define RENDERGL_LIGHTUNIFORMBLOCK_H

#include "glm/vec3.hpp"

struct LightUniformBlock{
    glm::vec3 lightColor = glm::vec3(0.0, 0.0, 1.0);
};

#endif //RENDERGL_LIGHTUNIFORMBLOCK_H
