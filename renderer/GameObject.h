#ifndef RENDERGL_GAMEOBJECT_H
#define RENDERGL_GAMEOBJECT_H

#include "Mesh.h"
#include "Transform.h"

class GameObject {
public:
    Mesh* mesh;
    Material* material;
    Material* depthMaterial;
    Transform transform;
    void Draw();
    void DrawDepth();
};

void GameObject::Draw(){
    this->material->Draw(this->transform);
}

void GameObject::DrawDepth(){
    this->depthMaterial->Draw(this->transform);
}

#endif //RENDERGL_GAMEOBJECT_H
