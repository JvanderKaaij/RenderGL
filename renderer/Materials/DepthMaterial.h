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

    void Draw(Transform transform) override{
        glUseProgram(this->programID);

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
        ModelMatrix = glm::scale(ModelMatrix, transform.scale);
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        GLuint lightSpaceMatrixLocation = glGetUniformLocation(this->programID, "lightProjectionMatrix");
        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(Scene::LightProjectionMatrix));

        GLuint m_location = glGetUniformLocation(this->programID, "model");
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    }
};
#endif //RENDERGL_DEPTHMATERIAL_H
