#include <vector>

#ifndef RENDERGL_MESH_H
#define RENDERGL_MESH_H

struct aiMaterial; //I need to forward declare aiMaterial here for some reason

struct Mesh {
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;
    aiMaterial* meshMaterial;
};


#endif