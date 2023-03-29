#include <iostream>
#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "Transform.h"
#include "Misc/Helpers.h"
#include <math.h>

Transform transform = Transform{
        glm::vec3(0,0,0),
        glm::vec3(0,0,0),
        glm::vec3(1,1,1)
};

void Camera::Set(glm::vec3 position) {
    transform.position = position;
    viewMatrix = glm::translate(glm::mat4(1.0f), transform.position);
}

void Camera::Move(glm::vec3 dir) {
    //fmodf

    float x = glm::clamp(transform.rotation.x, -2.0f, 2.0f);
    float y = glm::clamp(transform.rotation.y, -2.0f, 2.0f);
    float z = glm::clamp(transform.rotation.z, -2.0f, 2.0f);

    glm::quat QuatAroundX = glm::quat(-x, glm::vec3(1.0,0.0,0.0));
    glm::quat QuatAroundY = glm::quat(y, glm::vec3(0.0,1.0,0.0));
    glm::quat QuatAroundZ = glm::quat(-z, glm::vec3(0.0,0.0,1.0));
    glm::quat finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;

    glm::vec3 ndir = glm::vec4(dir, 1.0) * mat4_cast(finalOrientation);

    std::cout << "New Direction:";
    Helpers::DebugVec(ndir);

    transform.position += ndir;

    viewMatrix = glm::translate(viewMatrix, ndir);
}

void Camera::Rotate(glm::vec3 dir) {
    yawRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    pitchRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    yawRotation = RotateAroundAxis(up, glm::radians(dir.x * 100.0f), yawRotation);
    pitchRotation = RotateAroundAxis(right, glm::radians(dir.y * 100.0f), pitchRotation);

    orientation = yawRotation * pitchRotation;
    transform.rotation += glm::eulerAngles(orientation);

    std::cout << "Rotation:";
    Helpers::DebugVec(transform.rotation);

    viewMatrix = mat4_cast(orientation) * viewMatrix; //Note that order of multiplication matters here!
}

glm::mat4 Camera::GetProjectionMatrix() {
    return this->projection;
}

glm::mat4 Camera::GetViewMatrix(){
    return viewMatrix;
}

glm::quat Camera::RotateAroundAxis(const glm::vec3& axis, float angle, const glm::quat& quat)
{
    glm::quat rotation = glm::angleAxis(angle, axis);
    return rotation * quat;
}