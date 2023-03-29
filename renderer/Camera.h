#ifndef RENDERGL_CAMERA_H
#define RENDERGL_CAMERA_H

#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

enum class CameraDirection{
    None = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera();

    Transform transform;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float rotationSpeed;

    void UpdateCameraDirection(double dx, double dy);
    void UpdateCameraPos(CameraDirection dir, double dt);
    void SetCameraPos(glm::vec3 pos);

    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
private:
    void UpdateCameraVectors();

    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
    glm::mat4 viewMatrix;
};


#endif //RENDERGL_CAMERA_H
