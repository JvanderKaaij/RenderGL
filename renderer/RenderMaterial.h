//
// Created by Joey on 22/02/2023.
//

#ifndef RENDERGL_RENDERMATERIAL_H
#define RENDERGL_RENDERMATERIAL_H

#include <string>
#include <utility>
#include "Material.h"

class RenderMaterial: public Material {
public:
    RenderMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw() override{
        glUseProgram(this->programID);

        GLuint mvp_location = glGetUniformLocation(this->programID, "mvp");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(Scene::CameraMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->renderedTextureID);
        GLuint renderLocation = glGetUniformLocation(this->programID, "renderTexture");
        glUniform1i(renderLocation, 0);

    }
};
#endif //RENDERGL_RENDERMATERIAL_H
