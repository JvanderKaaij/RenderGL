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
    static Mesh Process(const char* str);
};


#endif //RENDERGL_MESHPARSER_H
