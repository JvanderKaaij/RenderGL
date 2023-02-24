#ifndef RENDERGL_GAMEOBJECT_H
#define RENDERGL_GAMEOBJECT_H

#include "Mesh.h"
#include "Transform.h"

class GameObject {
public:
    Mesh* mesh;
    Material* material;
    Transform transform;
    void Draw();
};

void GameObject::Draw(){
    this->material->Draw(this->transform);
}

#endif //RENDERGL_GAMEOBJECT_H
