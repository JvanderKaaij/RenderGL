#include "MeshParser.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh MeshParser::Process(const char* path) {
    const aiScene* scene = aiImportFile( path,
                                         aiProcess_CalcTangentSpace     |
                                         aiProcess_Triangulate               |
                                         aiProcess_GenSmoothNormals               |
                                         aiProcess_JoinIdenticalVertices     |
                                         aiProcess_SortByPType);

    const aiMesh* mesh = scene->mMeshes[0];

    Mesh parsedMesh;

    std::cout << "Number of Vertices: " << mesh->mNumVertices << std::endl;

    for(unsigned int j = 0; j < mesh->mNumVertices; j++) {
        // Extract vertex position, normal, and texture coordinates
        parsedMesh.Vertices.push_back(mesh->mVertices[j].x);
        parsedMesh.Vertices.push_back(mesh->mVertices[j].y);
        parsedMesh.Vertices.push_back(mesh->mVertices[j].z);
        if(mesh->mNormals) {
            parsedMesh.Normals.push_back(mesh->mNormals[j].x);
            parsedMesh.Normals.push_back(mesh->mNormals[j].y);
            parsedMesh.Normals.push_back(mesh->mNormals[j].z);
        }
        if (mesh->HasTextureCoords(0)) {
            parsedMesh.TextureCoords.push_back(mesh->mTextureCoords[0][j].x);
            parsedMesh.TextureCoords.push_back(mesh->mTextureCoords[0][j].y);
        }
        for(unsigned int k = 0; k < mesh->mNumFaces; k++) {
            const aiFace& face = mesh->mFaces[k];
            for(unsigned int l = 0; l < face.mNumIndices; l++) {
                parsedMesh.Indices.push_back(face.mIndices[l]);
            }
        }
    }

    //What if there are more materials?
    parsedMesh.meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

    return parsedMesh;
}
