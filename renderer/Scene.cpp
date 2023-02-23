
#include "Scene.h"
#include "glm/detail/type_mat4x4.hpp"

DirectionalLight Scene::directional_light = DirectionalLight{glm::vec3(0,0,-1), 1.0f};
glm::mat4 Scene::CameraMatrix = glm::mat4(1.0f);