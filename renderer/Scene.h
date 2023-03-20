#ifndef RENDERGL_SCENE_H
#define RENDERGL_SCENE_H


#include "Lights/DirectionalLight.h"
#include "glm/fwd.hpp"
#include "Transform.h"
#include "glm/detail/type_mat4x4.hpp"
#include "LightUniformBlock.h"

class Scene {
public:
    static DirectionalLight directional_light;
    static LightUniformBlock LightBlock;
    static glm::mat4 ViewMatrix;
    static glm::mat4 ProjectionMatrix;
    static Transform CameraTransform;

private:
    static glm::mat4 LightProjection;
    static glm::mat4 LightView;
};


#endif //RENDERGL_SCENE_H
