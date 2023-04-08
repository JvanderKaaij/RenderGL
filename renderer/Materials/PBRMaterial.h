#ifndef RENDERGL_PBRMATERIAL_H
#define RENDERGL_PBRMATERIAL_H

#include <string>
#include "Material.h"

class PBRMaterial: public Material {
public:
    PBRMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}

    float roughness;
    float light_intensity;
    bool is_metal;
    glm::vec3 color;

    unsigned int diffuseTextID = -1;
    unsigned int metallicTextID = -1;
    unsigned int roughnessTextID = -1;
    unsigned int normalTextID = -1;
    unsigned int cubemapID;

    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{

        Material::Draw(transform, sceneUniforms);

        //UNIFORMS

        GLuint roughness_location = glGetUniformLocation(this->programID, "roughness");
        glUniform1f(roughness_location, this->roughness);

        GLuint light_intensity_location = glGetUniformLocation(this->programID, "lightIntensity");
        glUniform1f(light_intensity_location, this->light_intensity);

        GLuint is_metal_location = glGetUniformLocation(this->programID, "isMetal");
        glUniform1i(is_metal_location, true);

        GLuint color_location = glGetUniformLocation(this->programID, "color");
        glUniform3fv(color_location, 1, glm::value_ptr(this->color));

        //TEXTURE USED
        GLuint diffuse_used_location = glGetUniformLocation(this->programID, "diffuseTxtUsed");
        glUniform1i(diffuse_used_location, (this->diffuseTextID != -1));

        GLuint metallic_used_location = glGetUniformLocation(this->programID, "metallicTxtUsed");
        glUniform1i(metallic_used_location, (this->metallicTextID != -1));

        GLuint roughness_used_location = glGetUniformLocation(this->programID, "roughnessTxtUsed");
        glUniform1i(roughness_used_location, (this->roughnessTextID != -1));

        GLuint normal_used_location = glGetUniformLocation(this->programID, "normalTxtUsed");
        glUniform1i(normal_used_location, (this->normalTextID != -1));

        //TEXTURES

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseTextID);
        GLuint diffuse_texture_location = glGetUniformLocation(this->programID, "diffuseTexture");
        glUniform1i(diffuse_texture_location, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->metallicTextID);
        GLuint metallic_texture_location = glGetUniformLocation(this->programID, "metallicTexture");
        glUniform1i(metallic_texture_location, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, this->roughnessTextID);
        GLuint roughness_texture_location = glGetUniformLocation(this->programID, "roughnessTexture");
        glUniform1i(roughness_texture_location, 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, this->normalTextID);
        GLuint normal_texture_location = glGetUniformLocation(this->programID, "normalTexture");
        glUniform1i(normal_texture_location, 3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubemapID);
        GLuint skyboxLocation = glGetUniformLocation(this->programID, "skyboxTexture");
        glUniform1i(skyboxLocation, 4);
    }

};

#endif //RENDERGL_PBRMATERIAL_H
