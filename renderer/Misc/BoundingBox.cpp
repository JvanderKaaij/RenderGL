#include "BoundingBox.h"

void BoundingBox::FromAssimpMesh(const aiMesh& mesh) {

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

BoundingBox::BoundingBox() = default;
BoundingBox::~BoundingBox() = default;
