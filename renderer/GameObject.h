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
    
    void Draw(SceneUniformBlock* sceneUniforms);
    void DrawDepth(SceneUniformBlock* sceneUniforms);
};

void GameObject::Draw(SceneUniformBlock* sceneUniforms){
    this->material->Draw(this->transform, sceneUniforms);
    this->mesh->Draw();
}

void GameObject::DrawDepth(SceneUniformBlock* sceneUniforms){
    this->depthMaterial->Draw(this->transform, sceneUniforms);
    this->mesh->Draw();
}

#endif //RENDERGL_GAMEOBJECT_H
