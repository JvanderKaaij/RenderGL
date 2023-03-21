
#include "Scene.h"

DirectionalLight Scene::directional_light = DirectionalLight();

Scene::Scene() {
    this->camera = Camera();
}

SceneUniformBlock* Scene::GetSceneUniforms(){
    this->sceneUniforms = new SceneUniformBlock();
    //note that I parse to a vec4 here to make sure it works with UBO's and std140 (that expects type sizes to be divisible by 4)
    this->sceneUniforms->cameraPosition = glm::vec4(this->camera.transform.position.x, this->camera.transform.position.y, this->camera.transform.position.z, 0.0);
    this->sceneUniforms->cameraProjection = this->camera.GetProjectionMatrix();
    this->sceneUniforms->cameraView = this->camera.GetViewMatrix(this->camera.transform);
    return this->sceneUniforms;
}