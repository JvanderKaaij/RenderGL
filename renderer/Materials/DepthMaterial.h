#ifndef RENDERGL_DEPTHMATERIAL_H
#define RENDERGL_DEPTHMATERIAL_H

#include <string>
#include <utility>
#include "Material.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Scene.h"

class DepthMaterial: public Material {
public:
    DepthMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{
        glUseProgram(this->programID);

        GLuint lightSpaceMatrixLocation = glGetUniformLocation(this->programID, "lightProjectionMatrix");
        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(sceneUniforms->sceneLightProjection));

        GLuint m_location = glGetUniformLocation(this->programID, "model");
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(transform.matrix));

    }
};
#endif //RENDERGL_DEPTHMATERIAL_H
