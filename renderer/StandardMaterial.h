
#ifndef RENDERGL_STANDARDMATERIAL_H
#define RENDERGL_STANDARDMATERIAL_H

#include "Material.h"
#include "../interface/MaterialInterface.h"

class StandardMaterial: public Material {
public:
    StandardMaterial(std::string vertex_path, std::string fragment_path): Material(vertex_path, fragment_path){}

    void Draw() override{
        glUseProgram(this->programID);

        GLint time_location = glGetUniformLocation(this->programID, "timer");
        glUniform1f(time_location, (float)glfwGetTime());


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseID);
        GLuint diffuseLocation = glGetUniformLocation(this->programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularID);
        GLuint specularLocation = glGetUniformLocation(this->programID, "specularTexture");
        glUniform1i(specularLocation, 1);

    }
};
#endif //RENDERGL_STANDARDMATERIAL_H
