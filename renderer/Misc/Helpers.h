//
// Created by Joey on 29/03/2023.
//

#ifndef RENDERGL_HELPERS_H
#define RENDERGL_HELPERS_H

#include <iostream>
#include "glm/vec3.hpp"

class Helpers{
public:
    static void DebugVec(glm::vec3 vector){
        std::cout << "(" << vector.x << "," << vector.y << "," << vector.z << ")" << std::endl;
    }
};
#endif //RENDERGL_HELPERS_H
