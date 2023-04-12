#ifndef RENDERGL_SCENEIMPORTER_H
#define RENDERGL_SCENEIMPORTER_H

#include <vector>
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../renderer/Mesh.h"

class SceneImporter{
public:
    SceneImporter(const char *path);
    ~SceneImporter();
    std::vector<Mesh*> meshCollection;

private:
    Mesh* ParseMesh(const aiMesh& mesh);
};


#endif //RENDERGL_SCENEIMPORTER_H
