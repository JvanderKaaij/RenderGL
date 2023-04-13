//
// Created by Joey on 12/04/2023.
//

#ifndef RENDERGL_BOUNDINGBOX_H
#define RENDERGL_BOUNDINGBOX_H

#include <limits>

#include <assimp/mesh.h>
#include "../Mesh.h"
#include "glm/vec3.hpp"

class BoundingBox{
public:
    BoundingBox();
    ~BoundingBox();
    void FromAssimpMesh(const aiMesh& mesh);
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

#endif //RENDERGL_BOUNDINGBOX_H

