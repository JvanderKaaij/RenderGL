#ifndef RENDERGL_SCENE_H
#define RENDERGL_SCENE_H


#include "DirectionalLight.h"
#include "glm/fwd.hpp"
#include "Transform.h"
#include "glm/detail/type_mat4x4.hpp"

class Scene {
public:
    static DirectionalLight directional_light;
    static glm::mat4 ViewMatrix;
    static glm::mat4 ProjectionMatrix;
    static Transform CameraTransform;
    static glm::mat4 LightSpaceMatrix;
private:
    static glm::mat4 LightProjection;
    static glm::mat4 LightView;
};


#endif //RENDERGL_SCENE_H
