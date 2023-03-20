#ifndef RENDERGL_CAMERA_H
#define RENDERGL_CAMERA_H

#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "Transform.h"

class Camera {
public:
    glm::mat4 GetProjectionMatrix();
    glm::mat4 GetViewMatrix(Transform cameraTransform);
    Transform transform;
private:
    glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
};


#endif //RENDERGL_CAMERA_H
