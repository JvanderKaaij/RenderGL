#include <iostream>
#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "Transform.h"
#include "Misc/Helpers.h"
#include <math.h>


Camera::Camera() {
    transform = Transform{
            glm::vec3(0,0,0),
            glm::vec3(0,0,0),
            glm::vec3(1,1,1)
    };
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 12.5f;
    rotationSpeed = 40.0f;
    cameraFront = glm::vec3(0.0f, 0.0f, -8.0f);
    UpdateCameraVectors();
}

void Camera::SetCameraPos(glm::vec3 pos){
    transform.position = pos;
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::UpdateCameraDirection(double dx, double dy) {
    yaw += dx * rotationSpeed;
    pitch += dy * rotationSpeed;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    UpdateCameraVectors();
}

void Camera::UpdateCameraPos(CameraDirection dir, double dt) {
    float velocity = (float) dt * speed;
    switch(dir){
        case CameraDirection::FORWARD:
            transform.position += cameraFront * velocity;
            break;
        case CameraDirection::BACKWARD:
            transform.position -= cameraFront * velocity;
            break;
        case CameraDirection::RIGHT:
            transform.position += cameraRight * velocity;
            break;
        case CameraDirection::LEFT:
            transform.position -= cameraRight * velocity;
            break;
        case CameraDirection::UP:
            transform.position += cameraUp * velocity;
            break;
        case CameraDirection::DOWN:
            transform.position -= cameraUp * velocity;
            break;
    }
}

glm::mat4 Camera::GetProjectionMatrix() {
    return this->projection;
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(transform.position, transform.position + cameraFront, worldUp);
}


