
#include <iostream>
#include "Scene.h"
#include "GLFW/glfw3.h"

Scene::Scene() {
    this->camera = Camera();
    this->sceneUniforms = new SceneUniformBlock();
    this->directionalLight = DirectionalLight();
    this->directionalLight.transform.SetPosition(glm::vec3(0, 20, 0));
    this->directionalLight.transform.SetRotation(glm::vec3(0,0,0));
    this->selectedTransform = &this->directionalLight.transform;
}

SceneUniformBlock* Scene::SetSceneUniforms(){

    this->sceneUniforms->time = (float)glfwGetTime();

    //note that I parse to a vec4 here to make sure it works with UBO's and std140 (that expects type sizes to be divisible by 4)
    this->sceneUniforms->cameraPosition = glm::vec4(this->camera.transform.position.x, this->camera.transform.position.y, this->camera.transform.position.z, 0.0);
    this->sceneUniforms->cameraProjection = this->camera.GetProjectionMatrix();
    this->sceneUniforms->cameraView = this->camera.GetViewMatrix();
    glm::vec3 forward = this->directionalLight.transform.Forward() - this->directionalLight.transform.position;
    this->sceneUniforms->sceneLightDirection = glm::normalize(glm::vec4(forward.x, forward.y, forward.z, 0));
    this->sceneUniforms->sceneLightProjection = this->directionalLight.GetProjectionMatrix();

    return this->sceneUniforms;
}

bool intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& minPoint, const glm::vec3& maxPoint, float& tMin, float& tMax)
{
    glm::vec3 invRayDir = 1.0f / rayDir;

    glm::vec3 t1 = (minPoint - rayOrigin) * invRayDir;
    glm::vec3 t2 = (maxPoint - rayOrigin) * invRayDir;

    glm::vec3 tMinVec = glm::min(t1, t2);
    glm::vec3 tMaxVec = glm::max(t1, t2);

    tMin = glm::max(glm::max(tMinVec.x, tMinVec.y), tMinVec.z);
    tMax = glm::min(glm::min(tMaxVec.x, tMaxVec.y), tMaxVec.z);

    return tMax >= tMin;
}

GameObject* Scene::CheckObjectSelection(glm::vec2 mousePosition) {

    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, screenWidth, screenHeight);
    glm::vec3 mousePos = glm::unProject(glm::vec3(mousePosition.x, screenHeight - mousePosition.y, 0.0f), this->camera.GetViewMatrix(), this->camera.GetProjectionMatrix(), viewport);
    glm::vec3 rayOrigin = this->camera.transform.position;
    glm::vec3 rayDir = glm::normalize(mousePos - this->camera.transform.position);
    for (auto& go : backBufferObjects)
    {
        if(go->GetMesh()->boundingBox) {
            //TODO Add rotation to position
            auto *bbox = go->GetMesh()->boundingBox;
            glm::vec3 center = bbox->center + go->transform.GetPosition();
            glm::vec3 minPoint = center - bbox->extents;
            glm::vec3 maxPoint = center + bbox->extents;

            float tMin, tMax;
            if (intersects(rayOrigin, rayDir, minPoint, maxPoint, tMin, tMax)) {
                return go;
            }
        }
    }

    return nullptr;

}

