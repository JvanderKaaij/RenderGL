#include "DirectionalLight.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

glm::mat4 DirectionalLight::GetProjectionMatrix() {
    glm::mat4 LightProjection = glm::ortho(-45.0f, 45.0f, -45.0f, 45.0f, .001f, 80.0f);

    glm::mat4 LightView = glm::lookAt(this->transform.position,
                                      this->transform.Forward(),
                                      glm::vec3( 0.0f, 1.0f,  0.0f));

    return LightProjection * LightView;
}

void DirectionalLight::Update(){
    gizmoObjTransform->matrix = this->transform.matrix;
}
