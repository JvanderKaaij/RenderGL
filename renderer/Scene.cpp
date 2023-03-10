
#include "Scene.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

DirectionalLight Scene::directional_light = DirectionalLight{glm::vec3(0,1,-2), 1.0f};
glm::mat4 Scene::ViewMatrix = glm::mat4(1.0f);
glm::mat4 Scene::ProjectionMatrix = glm::mat4(1.0f);

Transform Scene::CameraTransform = Transform{
    glm::vec3(0,0,0),
    glm::vec3(0,0,0),
    glm::vec3(1,1,1)
};

glm::mat4 Scene::LightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 1.0f, 10.0f);
glm::mat4 Scene::LightView = glm::lookAt(Scene::directional_light.direction,
                                  glm::vec3( 0.0f, 0.0f,  0.0f),
                                  glm::vec3( 0.0f, 1.0f,  0.0f));


glm::mat4 Scene::LightSpaceMatrix = LightProjection * LightView;