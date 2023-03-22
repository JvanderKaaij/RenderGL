
#ifndef RENDERGL_STANDARDMATERIAL_H
#define RENDERGL_STANDARDMATERIAL_H


#include <utility>
#include "../../interface/MaterialInterface.h"
#include "../Scene.h"
#include "Material.h"

class StandardMaterial: public Material {
public:

    StandardMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{
        Material::Draw(transform, sceneUniforms); //call the draw function on the base class

        GLuint ambient_color_location = glGetUniformLocation(this->programID, "ambientColor");
        glUniform3fv(ambient_color_location, 1, glm::value_ptr(this->ambientColor));

        GLuint diffuse_color_location = glGetUniformLocation(this->programID, "diffuseColor");
        glUniform3fv(diffuse_color_location, 1, glm::value_ptr(this->diffuseColor));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseTextID);
        GLuint diffuseLocation = glGetUniformLocation(this->programID, "diffuseTexture");
        glUniform1i(diffuseLocation, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularTextID);
        GLuint specularLocation = glGetUniformLocation(this->programID, "specularTexture");
        glUniform1i(specularLocation, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, this->shadowMapID);
        GLuint shadowMapLocation = glGetUniformLocation(this->programID, "shadowMapTexture");
        glUniform1i(shadowMapLocation, 3);

        GLuint uniformBlockIndex = glGetUniformBlockIndex(this->programID, "SceneUniformBlock");
        glUniformBlockBinding(this->programID, uniformBlockIndex, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->sceneBlockUBO);

    }
};
#endif //RENDERGL_STANDARDMATERIAL_H
