
#ifndef RENDERGL_SKYBOXMATERIAL_H
#define RENDERGL_SKYBOXMATERIAL_H

#include <utility>
#include "../../interface/MaterialInterface.h"
#include "../Scene.h"
#include "GLFW/glfw3.h"
#include "Material.h"
#include "glm/gtc/type_ptr.hpp"

class SkyboxMaterial: public Material {
public:

    SkyboxMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw(Transform transform) override{
        glUseProgram(this->programID);

        glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 0);

        GLuint v_location = glGetUniformLocation(this->programID, "view");
        glUniformMatrix4fv(v_location, 1, GL_FALSE, glm::value_ptr(glm::inverse(Scene::ViewMatrix)));

        GLuint p_location = glGetUniformLocation(this->programID, "projection");
        glUniformMatrix4fv(p_location, 1, GL_FALSE, glm::value_ptr(Scene::ProjectionMatrix));

    }
};
#endif //RENDERGL_SKYBOXMATERIAL_H
