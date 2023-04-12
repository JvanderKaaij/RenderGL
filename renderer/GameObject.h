#ifndef RENDERGL_GAMEOBJECT_H
#define RENDERGL_GAMEOBJECT_H

#include "Mesh.h"
#include "Transform.h"

class GameObject {
public:
    Material* material;
    Material* depthMaterial;
    Transform transform;
    
    void SetMesh(Mesh* _mesh);
    void Draw(SceneUniformBlock* sceneUniforms);
    void DrawDepth(SceneUniformBlock* sceneUniforms);
private:
    Mesh* mesh;
};

void GameObject::Draw(SceneUniformBlock* sceneUniforms){
    this->material->Draw(this->transform, sceneUniforms);
    this->mesh->Draw();
}

void GameObject::DrawDepth(SceneUniformBlock* sceneUniforms){
    this->depthMaterial->Draw(this->transform, sceneUniforms);
    this->mesh->Draw();
}

void GameObject::SetMesh(Mesh *_mesh) {
    mesh = _mesh;
}

#endif //RENDERGL_GAMEOBJECT_H
