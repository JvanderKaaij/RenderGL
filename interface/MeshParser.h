#ifndef RENDERGL_MESHPARSER_H
#define RENDERGL_MESHPARSER_H


#include <vector>
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../renderer/Mesh.h"

class MeshParser {
public:
    static void Process(const char* str, Mesh* parsedMesh);

};


#endif //RENDERGL_MESHPARSER_H
