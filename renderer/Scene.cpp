
#include <iostream>
#include "Scene.h"
#include "GLFW/glfw3.h"

Scene::Scene() {
    this->camera = Camera();
    this->sceneUniforms = new SceneUniformBlock();
    this->directionalLight = DirectionalLight();
//    this->directionalLight.transform.SetPosition(glm::vec3(2, 3, 3));
//    this->directionalLight.transform.SetRotation(glm::vec3(-1,-1,-1));
}

SceneUniformBlock* Scene::SetSceneUniforms(){

    this->sceneUniforms->time = (float)glfwGetTime();

    //note that I parse to a vec4 here to make sure it works with UBO's and std140 (that expects type sizes to be divisible by 4)
    this->sceneUniforms->cameraPosition = glm::vec4(this->camera.transform.position.x, this->camera.transform.position.y, this->camera.transform.position.z, 0.0);
    this->sceneUniforms->cameraProjection = this->camera.GetProjectionMatrix();
    this->sceneUniforms->cameraView = this->camera.GetViewMatrix();

    this->sceneUniforms->sceneLightDirection = glm::normalize(glm::vec4(this->directionalLight.transform.rotation.x, this->directionalLight.transform.rotation.y, this->directionalLight.transform.rotation.z, 0));
    this->sceneUniforms->sceneLightProjection = this->directionalLight.GetProjectionMatrix();

    return this->sceneUniforms;
}