
#include "Scene.h"

DirectionalLight Scene::directional_light = DirectionalLight();
glm::mat4 Scene::ViewMatrix = glm::mat4(1.0f);
glm::mat4 Scene::ProjectionMatrix = glm::mat4(1.0f);

Transform Scene::CameraTransform = Transform{
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(1,1,1)
};

LightUniformBlock Scene::LightBlock = LightUniformBlock();