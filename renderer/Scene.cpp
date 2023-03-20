
#include "Scene.h"

DirectionalLight Scene::directional_light = DirectionalLight();

Scene::Scene() {
    this->camera = Camera();
}

SceneUniformBlock* Scene::GetSceneUniforms(){
    this->sceneUniforms = new SceneUniformBlock();
    this->sceneUniforms->cameraPosition = this->camera.transform.position;
    this->sceneUniforms->cameraProjection = this->camera.GetProjectionMatrix();
    this->sceneUniforms->cameraView = this->camera.GetViewMatrix(this->camera.transform);
    return this->sceneUniforms;
}