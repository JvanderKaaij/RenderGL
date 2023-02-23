#include <vector>
#include "Material.h"

#ifndef RENDERGL_MESH_H
#define RENDERGL_MESH_H

struct aiMaterial; //I need to forward declare aiMaterial here for some reason

class Mesh {
public:
    Mesh(const char* path);
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;
    unsigned int vaoID;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint textureCoordBuffer;

    aiMaterial* meshMaterialData;
    Material* material;
};


#endif