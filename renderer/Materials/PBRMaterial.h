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

    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{

        Material::Draw(transform, sceneUniforms);

        GLuint roughness_location = glGetUniformLocation(this->programID, "roughness");
        glUniform1f(roughness_location, this->roughness);

        GLuint light_intensity_location = glGetUniformLocation(this->programID, "light_intensity");
        glUniform1f(light_intensity_location, this->light_intensity);

        GLuint is_metal_location = glGetUniformLocation(this->programID, "is_metal");
        glUniform1i(is_metal_location, this->is_metal);

        GLuint color_location = glGetUniformLocation(this->programID, "color");
        glUniform3fv(color_location, 1, glm::value_ptr(this->color));
    }

};

#endif //RENDERGL_PBRMATERIAL_H
