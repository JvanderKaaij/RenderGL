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
    void Move(glm::vec3 dir);
    void Set(glm::vec3 position);
    void Rotate(glm::vec3 dir);
    glm::quat RotateAroundAxis(const glm::vec3& axis, float angle, const glm::quat& quat);
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
    Transform transform;
    glm::quat orientation;
private:
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
    glm::mat4 viewMatrix;

    glm::quat yawRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::quat pitchRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 forward = glm::vec3(0,0,-1);
    glm::vec3 up = glm::vec3(0,1,0);
    glm::vec3 right = glm::vec3(1,0,0);

    glm::vec3 upWorld = glm::vec3(0,1,0);
};


#endif //RENDERGL_CAMERA_H
