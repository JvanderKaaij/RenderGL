#include <vector>
#include "Material.h"

#ifndef RENDERGL_MESH_H
#define RENDERGL_MESH_H

struct aiMaterial; //I need to forward declare aiMaterial here for some reason

struct Mesh {
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;

    unsigned int vaoID;

    aiMaterial* meshMaterialData;
    Material* material;
    unsigned int programID;

    unsigned int diffuseID;
    unsigned int specularID;
};


#endif