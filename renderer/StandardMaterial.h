
#ifndef RENDERGL_STANDARDMATERIAL_H
#define RENDERGL_STANDARDMATERIAL_H

#include "Material.h"
#include "../interface/MaterialInterface.h"

class StandardMaterial: public Material {
public:
    StandardMaterial(std::string vertex_path, std::string fragment_path): Material(vertex_path, fragment_path){
        std::cout << "Material created from Standard Material" << std::endl;
    }

    void Draw() override{
        std::cout << "Draw from Standard Mat" << std::endl;
        glUseProgram(this->programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseID);
        GLuint diffuseLocation = glGetUniformLocation(this->programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 0);
    }
};
#endif //RENDERGL_STANDARDMATERIAL_H
