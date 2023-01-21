#include <vector>

struct ParsedMesh {
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<unsigned int> Indices;
};