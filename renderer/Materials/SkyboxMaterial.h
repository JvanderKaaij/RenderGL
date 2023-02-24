
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
        glm::mat4 ModelMatrix = glm::rotate(glm::mat4(1.0f), transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));

        GLuint mvp_location = glGetUniformLocation(this->programID, "mvp");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

    }
};
#endif //RENDERGL_SKYBOXMATERIAL_H
