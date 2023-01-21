#ifndef RENDERGL_MESHPARSER_H
#define RENDERGL_MESHPARSER_H


#include <vector>
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ParsedMesh.h"

class MeshParser {
public:
    static ParsedMesh Process(const char* str);
};


#endif //RENDERGL_MESHPARSER_H
