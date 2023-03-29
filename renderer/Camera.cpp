#include <iostream>
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
    yawRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    pitchRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Rotate quaternion around axis by 45 degrees
    yawRotation = RotateAroundAxis(up, glm::radians(dir.x * 100.0f), yawRotation);
    pitchRotation = RotateAroundAxis(right * yawRotation, glm::radians(dir.y * 100.0f), pitchRotation);

    glm::quat rot = yawRotation * pitchRotation;
    transform.rotation += glm::eulerAngles(rot) / (float)(2.0f * M_PI);
    viewMatrix = mat4_cast(rot) * viewMatrix; //Note that order of multiplication matters here!
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

glm::quat Camera::RotateAroundAxis(const glm::vec3& axis, float angle, const glm::quat& quat)
{
    glm::quat rotation = glm::angleAxis(angle, axis);
    return rotation * quat;
}