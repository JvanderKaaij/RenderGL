#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "Transform.h"


Transform transform = Transform{
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(1,1,1)
};


void Camera::Move(glm::vec3 dir) {
    transform.position += dir;
    viewMatrix = glm::translate(glm::mat4(1.0f), transform.position);
}

void Camera::Rotate(glm::vec3 dir) {
    glm::mat3 Translation = glm::translate(glm::mat4(1.0f), -transform.position);

    glm::quat QuatAroundX = glm::quat(dir.x, glm::vec3(1.0,0.0,0.0));
    glm::quat QuatAroundY = glm::quat(dir.y, glm::vec3(0.0,1.0,0.0));
    glm::quat QuatAroundZ = glm::quat(dir.z, glm::vec3(0.0,0.0,1.0));
    glm::quat finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
    glm::quat currentOrientation = glm::quat(transform.rotation);

    glm::quat orientation = currentOrientation * finalOrientation;

    viewMatrix *= glm::translate(glm::mat4(1.0f), -transform.position) * mat4_cast(orientation) * glm::translate(glm::mat4(1.0f), transform.position);

//    glm::mat3 TranslationBack = glm::translate(glm::mat4(1.0f), -transform.position);
//
//    glm::mat3 rot = Translation = Translation * mat3_cast(finalOrientation) * TranslationBack;
//
//    transform.rotation += glm::vec3(0.01) * finalOrientation;
}

glm::mat4 Camera::GetProjectionMatrix() {
    return this->projection;
}

glm::mat4 Camera::GetViewMatrix(){
//    glm::mat4 ViewRotate = glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
//    ViewRotate = glm::rotate(ViewRotate, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
//    ViewRotate = glm::rotate(ViewRotate, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
//
//    glm::mat4 ViewTranslate = glm::translate(ViewRotate, transform.position);

    return viewMatrix;
}