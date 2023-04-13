#include "GameObject.h"

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

Mesh* GameObject::GetMesh() {
    return mesh;
}