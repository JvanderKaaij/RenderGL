
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

    unsigned int cubemapID;

    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{
        glUseProgram(this->programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 0);

        GLuint v_location = glGetUniformLocation(this->programID, "view");
        glUniformMatrix4fv(v_location, 1, GL_FALSE, glm::value_ptr(glm::inverse(sceneUniforms->cameraView)));

        GLuint p_location = glGetUniformLocation(this->programID, "projection");
        glUniformMatrix4fv(p_location, 1, GL_FALSE, glm::value_ptr(sceneUniforms->cameraProjection));

    }
};
#endif //RENDERGL_SKYBOXMATERIAL_H
