
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

//        std::cout << "Draw Skybox: " << this->cubemapID << std::endl;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 0);
    }
};
#endif //RENDERGL_SKYBOXMATERIAL_H
