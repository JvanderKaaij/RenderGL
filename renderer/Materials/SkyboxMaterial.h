
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

    void Draw() override{
        std::cout << "Draw from SkyboxMaterial with program ID: " << this->programID << std::endl;
        glUseProgram(this->programID);


    }
};
#endif //RENDERGL_SKYBOXMATERIAL_H
