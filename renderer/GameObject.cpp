#include "GameObject.h"

GameObject::GameObject(std::string _name) {
    this->name = std::move(_name);
}

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
