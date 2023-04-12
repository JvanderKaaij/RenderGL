//
// Created by Joey on 12/04/2023.
//

#ifndef RENDERGL_BOUNDINGBOX_H
#define RENDERGL_BOUNDINGBOX_H

#include <limits>
#include <assimp/mesh.h>
#include "../Mesh.h"

class BoundingBox{
public:
    void FromAssimpMesh(aiMesh mesh);
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
    glm::vec3 center;
    glm::vec3 extents;
};

void BoundingBox::FromAssimpMesh(aiMesh mesh) {

    for (int i = 0; i < mesh.mNumVertices; i++)
    {
        if (mesh.mVertices[i].x < minX) minX = mesh.mVertices[i].x;
        if (mesh.mVertices[i].y < minY) minY = mesh.mVertices[i].y;
        if (mesh.mVertices[i].z < minZ) minZ = mesh.mVertices[i].z;
        if (mesh.mVertices[i].x > maxX) maxX = mesh.mVertices[i].x;
        if (mesh.mVertices[i].y > maxY) maxY = mesh.mVertices[i].y;
        if (mesh.mVertices[i].z > maxZ) maxZ = mesh.mVertices[i].z;
    }
   minPoint = glm::vec3(minX, minY, minZ);
   maxPoint = glm::vec3(maxX, maxY, maxZ);
   center = (minPoint + maxPoint) / 2.0f;
   extents = (maxPoint - minPoint) / 2.0f;
}

#endif //RENDERGL_BOUNDINGBOX_H
