
#include <string>
#include "Material.h"
#include "../interface/MaterialInterface.h"

Material::Material(std::string vertex_path, std::string fragment_path) {
    std::cout << "Material created from base" << std::endl;
    this->shaders = MaterialInterface::CompileShaders(vertex_path.c_str(), fragment_path.c_str());
    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->shaders.vertShader);
    glAttachShader(this->programID, this->shaders.fragShader);
    glLinkProgram(this->programID);
    glUseProgram(this->programID);
}

void Material::Draw() {
    std::cout << "Draw from Material" << std::endl;
}
