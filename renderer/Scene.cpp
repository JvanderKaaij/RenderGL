
#include <iostream>
#include "Scene.h"
#include "GLFW/glfw3.h"

Scene::Scene() {
    this->camera = Camera();
    this->sceneUniforms = new SceneUniformBlock();
}

SceneUniformBlock* Scene::SetSceneUniforms(){

    this->sceneUniforms->time = (float)glfwGetTime();

    //note that I parse to a vec4 here to make sure it works with UBO's and std140 (that expects type sizes to be divisible by 4)
    this->sceneUniforms->cameraPosition = glm::vec4(this->camera.transform.position.x, this->camera.transform.position.y, this->camera.transform.position.z, 0.0);
    this->sceneUniforms->cameraProjection = this->camera.GetProjectionMatrix();
    this->sceneUniforms->cameraView = this->camera.GetViewMatrix(this->camera.transform);

    this->sceneUniforms->sceneLightDirection = glm::normalize(-glm::vec4(this->directional_light.direction.x, this->directional_light.direction.y, this->directional_light.direction.z, 0));
    this->sceneUniforms->sceneLightProjection = this->directional_light.GetProjectionMatrix();

    return this->sceneUniforms;
}