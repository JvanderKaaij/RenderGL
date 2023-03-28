#ifndef RENDERGL_CAMERA_H
#define RENDERGL_CAMERA_H

#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

class Camera {
public:
    void Move(glm::vec3 dir);
    void Rotate(glm::vec3 dir);
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix();
    Transform transform;
    glm::quat orientation;
private:
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
    glm::mat4 viewMatrix;
};


#endif //RENDERGL_CAMERA_H
