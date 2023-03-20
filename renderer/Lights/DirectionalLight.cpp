#include "DirectionalLight.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "../Scene.h"

glm::mat4 DirectionalLight::GetProjectionMatrix() {
    glm::mat4 LightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);
    glm::mat4 LightView = glm::lookAt((glm::vec3(-Scene::directional_light.direction.x*10.0f, -Scene::directional_light.direction.y*10.0f, -Scene::directional_light.direction.z*10.0f)),
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3( 0.0f, 1.0f,  0.0f));

    return LightProjection * LightView;
}
