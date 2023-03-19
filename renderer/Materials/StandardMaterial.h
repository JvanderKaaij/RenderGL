
#ifndef RENDERGL_STANDARDMATERIAL_H
#define RENDERGL_STANDARDMATERIAL_H


#include <utility>
#include "../../interface/MaterialInterface.h"
#include "../Scene.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"
#include "Material.h"

class StandardMaterial: public Material {
public:

    StandardMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw(Transform transform) override{
        glUseProgram(this->programID);

        GLint time_location = glGetUniformLocation(this->programID, "timer");
        glUniform1f(time_location, (float)glfwGetTime());

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
        ModelMatrix = glm::scale(ModelMatrix, transform.scale);
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        GLuint m_location = glGetUniformLocation(this->programID, "model");
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        GLuint v_location = glGetUniformLocation(this->programID, "view");
        glUniformMatrix4fv(v_location, 1, GL_FALSE, glm::value_ptr(Scene::ViewMatrix));

        GLuint p_location = glGetUniformLocation(this->programID, "projection");
        glUniformMatrix4fv(p_location, 1, GL_FALSE, glm::value_ptr(Scene::ProjectionMatrix));

        GLint directional_light_location = glGetUniformLocation(this->programID, "directionalLight");
        glUniform3fv(directional_light_location, 1, glm::value_ptr(Scene::directional_light.direction));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseID);
        GLuint diffuseLocation = glGetUniformLocation(this->programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularID);
        GLuint specularLocation = glGetUniformLocation(this->programID, "specularTexture");
        glUniform1i(specularLocation, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 2);

        GLuint cam_location = glGetUniformLocation(this->programID, "cameraPosition");
        glUniform3fv(cam_location, 1, glm::value_ptr(Scene::CameraTransform.position));

        GLuint diffuse_color_location = glGetUniformLocation(this->programID, "diffuseColor");
        glUniform3fv(diffuse_color_location, 1, glm::value_ptr(this->diffuseColor));

        GLuint lightSpaceMatrixLocation = glGetUniformLocation(this->programID, "lightProjectionMatrix");
        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(Scene::GetLightProjectionMatrix()));

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, this->shadowMapID);
        GLuint shadowMapLocation = glGetUniformLocation(this->programID, "shadowMapTexture");
        glUniform1i(shadowMapLocation, 3);

        GLuint uniformBlockIndex = glGetUniformBlockIndex(this->programID, "LightBlock");
        glUniformBlockBinding(this->programID, uniformBlockIndex, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->lightBlockUBO);

    }
};
#endif //RENDERGL_STANDARDMATERIAL_H
