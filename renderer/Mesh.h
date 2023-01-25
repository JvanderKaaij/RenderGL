#include <vector>

struct Mesh {
    std::vector<float> Vertices;
    std::vector<float> Normals;
    std::vector<float> TextureCoords;
    std::vector<unsigned int> Indices;
    unsigned char* DiffuseTexture;
    int diffTextWidth, diffTextHeight, diffTextNrChannels;
};