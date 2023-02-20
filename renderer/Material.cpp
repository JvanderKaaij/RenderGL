
#include <string>
#include "Material.h"
#include "../interface/MaterialInterface.h"

Material::Material(std::string vertex_path, std::string fragment_path) {
    this->shaders = MaterialInterface::CompileShaders(vertex_path.c_str(), fragment_path.c_str());
}

Material::Material() {

}

void Material::Draw() {
    std::cout << "Draw from Material" << std::endl;
}
