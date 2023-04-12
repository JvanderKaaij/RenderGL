//
// Created by Joey on 12/04/2023.
//

#include "SceneImporter.h"


SceneImporter::SceneImporter(const char *path){

    const aiScene* scene = aiImportFile( path,
                                         aiProcess_CalcTangentSpace     |
                                         aiProcess_Triangulate               |
                                         aiProcess_GenSmoothNormals               |
                                         aiProcess_JoinIdenticalVertices     |
                                         aiProcess_SortByPType);

    for(unsigned int i=0;i<scene->mNumMeshes;i++){
        meshCollection.push_back(ParseMesh(*scene->mMeshes[i]));
    }

}

Mesh* SceneImporter::ParseMesh(const aiMesh& mesh) {
    Mesh* result_mesh = new Mesh();
    std::cout << "Number of Vertices: " << mesh.mNumVertices << std::endl;

    for(unsigned int j = 0; j < mesh.mNumVertices; j++) {
        // Extract vertex position, normal, and texture coordinates
        result_mesh->Vertices.push_back(mesh.mVertices[j].x);
        result_mesh->Vertices.push_back(mesh.mVertices[j].y);
        result_mesh->Vertices.push_back(mesh.mVertices[j].z);

        if(mesh.mNormals) {
            result_mesh->Normals.push_back(mesh.mNormals[j].x);
            result_mesh->Normals.push_back(mesh.mNormals[j].y);
            result_mesh->Normals.push_back(mesh.mNormals[j].z);
        }
        if (mesh.HasTextureCoords(0)) {
            result_mesh->TextureCoords.push_back(mesh.mTextureCoords[0][j].x);
            result_mesh->TextureCoords.push_back(mesh.mTextureCoords[0][j].y);
        }
    }

    for(unsigned int k = 0; k < mesh.mNumFaces; k++) {
        const aiFace& face = mesh.mFaces[k];
        for(unsigned int l = 0; l < face.mNumIndices; l++) {
            result_mesh->Indices.push_back(face.mIndices[l]);
        }
    }

    std::cout << "Number of Elements in Vertex Array: " << result_mesh->Indices.size() << std::endl;

    result_mesh->InitBuffers();

    return result_mesh;
}

SceneImporter::~SceneImporter() = default;
