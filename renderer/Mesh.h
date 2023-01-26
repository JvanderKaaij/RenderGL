#include <vector>
#include "Material.h"

struct Mesh {
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;
    aiMaterial* meshMaterial;
};