#ifndef RENDERGL_RENDERMATERIAL_H
#define RENDERGL_RENDERMATERIAL_H

#include <string>
#include <utility>
#include "Material.h"

class RenderMaterial: public Material {
public:
    RenderMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw(Transform transform) override{
        glUseProgram(this->programID);

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position);

        GLuint m_location = glGetUniformLocation(this->programID, "model");
        glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        GLuint v_location = glGetUniformLocation(this->programID, "view");
        glUniformMatrix4fv(v_location, 1, GL_FALSE, glm::value_ptr(Scene::ViewMatrix));

        GLuint p_location = glGetUniformLocation(this->programID, "projection");
        glUniformMatrix4fv(p_location, 1, GL_FALSE, glm::value_ptr(Scene::ProjectionMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->renderedTextureID);
        GLuint renderLocation = glGetUniformLocation(this->programID, "renderTexture");
        glUniform1i(renderLocation, 0);

    }
};
#endif //RENDERGL_RENDERMATERIAL_H
