#ifndef RENDERGL_GAMEOBJECT_H
#define RENDERGL_GAMEOBJECT_H

#include "Mesh.h"
#include "Transform.h"
#include "Materials/Material.h"

class GameObject {
public:
    std::string name;

    Material* material{};
    Material* depthMaterial{};
    Transform transform;
    GameObject(std::string name);
    void SetMesh(Mesh* _mesh);
    Mesh* GetMesh();
    void Draw(SceneUniformBlock* sceneUniforms);
    void DrawDepth(SceneUniformBlock* sceneUniforms);
private:
    Mesh* mesh{};
};



#endif //RENDERGL_GAMEOBJECT_H
