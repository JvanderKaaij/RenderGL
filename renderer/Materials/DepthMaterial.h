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

        float near_plane = 1.0f, far_plane = 10.0f;
        glm::mat4 lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(Scene::directional_light.direction,
                                          glm::vec3( 0.0f, 0.0f,  0.0f),
                                          glm::vec3( 0.0f, 1.0f,  0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        GLuint lightSpaceMatrixLocation = glGetUniformLocation(this->programID, "lightSpaceMatrix");
        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        GLuint m_location = glGetUniformLocation(this->programID, "model");
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    }
};
#endif //RENDERGL_DEPTHMATERIAL_H
