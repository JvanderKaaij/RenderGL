#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "Transform.h"


Transform transform = Transform{
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(1,1,1)
};

glm::mat4 Camera::GetProjectionMatrix() {
    return this->projection;
}

glm::mat4 Camera::GetViewMatrix(Transform cameraTransform){
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), cameraTransform.position);
    glm::mat4 ViewRotate = glm::rotate(ViewTranslate, cameraTransform.rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    ViewRotate = glm::rotate(ViewRotate, cameraTransform.rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    return ViewRotate;
}