#ifndef RENDERGL_PBRMATERIAL_H
#define RENDERGL_PBRMATERIAL_H

#include <string>
#include "Material.h"

class PBRMaterial: public Material {
public:
    PBRMaterial(std::string vertex_path, std::string fragment_path): Material(std::move(vertex_path), std::move(fragment_path)){}
    void Draw(Transform transform, SceneUniformBlock* sceneUniforms) override{

    }

};

#endif //RENDERGL_PBRMATERIAL_H
