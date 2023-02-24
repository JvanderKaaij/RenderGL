#ifndef RENDERGL_SCENE_H
#define RENDERGL_SCENE_H


#include "DirectionalLight.h"
#include "glm/fwd.hpp"

class Scene {
public:
    static DirectionalLight directional_light;
    static glm::mat4 ViewMatrix;
};


#endif //RENDERGL_SCENE_H
