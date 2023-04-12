

#ifndef RENDERGL_MESH_H
#define RENDERGL_MESH_H

#include <vector>
#include "Materials/Material.h"
#include "Misc/BoundingBox.h"

class BoundingBox;

class Mesh {
public:
    void Parse(const char* path);
    void Draw();
    void InitBuffers();

    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;

    BoundingBox* boundingBox;

    unsigned int vaoID;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint textureCoordBuffer;

};


#endif