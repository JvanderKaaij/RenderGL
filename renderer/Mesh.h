

#ifndef RENDERGL_MESH_H
#define RENDERGL_MESH_H

#include <vector>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
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

    unsigned int vertexBuffer;
    unsigned int normalBuffer;
    unsigned int textureCoordBuffer;

};


#endif