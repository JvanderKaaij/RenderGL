#ifndef RENDERGL_GAMEOBJECT_H
#define RENDERGL_GAMEOBJECT_H

#include "Mesh.h"

class GameObject {
public:
    Mesh* mesh;
    void Draw();
};


#endif //RENDERGL_GAMEOBJECT_H
