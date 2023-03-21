#ifndef RENDERGL_SCENE_H
#define RENDERGL_SCENE_H


#include "Lights/DirectionalLight.h"
#include "glm/fwd.hpp"
#include "Transform.h"
#include "glm/detail/type_mat4x4.hpp"
#include "SceneUniformBlock.h"
#include "Camera.h"

class Scene {
public:
    Scene();
    Camera camera;
    DirectionalLight directional_light;
    SceneUniformBlock* GetSceneUniforms();

private:
    static glm::mat4 LightProjection;
    static glm::mat4 LightView;
    SceneUniformBlock* sceneUniforms;
};


#endif //RENDERGL_SCENE_H
