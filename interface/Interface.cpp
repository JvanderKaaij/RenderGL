#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "MeshParser.h"
#include "InputInterface.h"
#include "../renderer/Lights/DirectionalLight.h"
#include "MaterialInterface.h"
#include "../renderer/Materials/Material.h"
#include "../renderer/Materials/StandardMaterial.h"
#include "../renderer/Materials/RenderMaterial.h"
#include "../renderer/Materials/SkyboxMaterial.h"
#include "../renderer/Materials/DepthMaterial.h"
#include "../renderer/Buffers/FrameBuffer.h"
#include "../renderer/Buffers/DepthFrameBuffer.h"
#include "../renderer/Materials/PBRMaterial.h"
#include "SceneImporter.h"
#include "../gui/MainGUI.h"
#include <glm/gtx/euler_angles.hpp>

GLFWwindow* window;
static bool throw_exit = false;

float deltaTime = 0.0f;
float lastTime = 0.0f;

FrameBuffer* fb;
DepthFrameBuffer* directional_light_shadow_map;

std::vector<GameObject*> skyboxBufferObjects = std::vector<GameObject*>();

Scene scene = Scene();
InputInterface* inputInterface;

MainGUI mainGui = MainGUI();

//This file should be the RenderInterface
//And inputs should be registered at an over-arching engine class

void registerInputs(GLFWwindow* window){
    inputInterface = new InputInterface(window, &scene);
    inputInterface->Subscribe(GLFW_KEY_A, [](){ scene.camera.UpdateCameraPos(CameraDirection::LEFT, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_D, [](){ scene.camera.UpdateCameraPos(CameraDirection::RIGHT, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_W, [](){ scene.camera.UpdateCameraPos(CameraDirection::FORWARD, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_S, [](){ scene.camera.UpdateCameraPos(CameraDirection::BACKWARD, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_E, [](){ scene.camera.UpdateCameraPos(CameraDirection::UP, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_Q, [](){ scene.camera.UpdateCameraPos(CameraDirection::DOWN, deltaTime); });
    inputInterface->Subscribe(GLFW_KEY_ESCAPE, [=](){throw_exit = true;});
    inputInterface->InitKeyCallback();
}

Mesh* initMesh(const std::string& path){
    auto* mesh = new Mesh();
    mesh->Parse(path.c_str());
    mesh->InitBuffers();
    return mesh;
}

GameObject* initGameObject(std::string name){
    auto* go = new GameObject(name);
    return go;
}

void initSceneUniformBlock(){
    glGenBuffers(1, &scene.scene_ubo_id);
    glBindBuffer(GL_UNIFORM_BUFFER, scene.scene_ubo_id);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneUniformBlock), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, scene.scene_ubo_id);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneUniformBlock), scene.sceneUniforms);
}

void drawFrameBuffer(FrameBuffer* buffer){
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->id);
    glViewport(0,0,buffer->texture->width,buffer->texture->width);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto gObj : scene.backBufferObjects){
        gObj->Draw(scene.sceneUniforms);
    }
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,scene.screenWidth,scene.screenHeight);

    for(auto gObj : scene.backBufferObjects){
        gObj->Draw(scene.sceneUniforms);
    }
}

void drawShadowBuffer(){

    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, directional_light_shadow_map->id);
    glViewport(0, 0, directional_light_shadow_map->texture->width, directional_light_shadow_map->texture->width);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(auto gObj : scene.backBufferObjects){
        if(gObj->material->castShadow){
            gObj->DrawDepth(scene.sceneUniforms);
        }
    }

}

void drawSkyboxBuffer(){
    glDepthMask(GL_FALSE);
    for(auto gObj : skyboxBufferObjects){
        gObj->Draw(scene.sceneUniforms);
    }
    glDepthMask(GL_TRUE);
}


void draw(){
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    //handle interface events
    glfwPollEvents();

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneUniformBlock), scene.SetSceneUniforms());
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSkyboxBuffer();
    drawShadowBuffer();
    drawFrameBuffer(fb);
    drawBackBuffer();

    //UI
    mainGui.Draw(&scene);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    if (mainGui.blockMouseMove) {
        // Scroll event should be handled by Dear ImGui, not OpenGL
        return;
    }
    inputInterface->Update(window);

}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int init() {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(scene.screenWidth, scene.screenHeight, "Hello Teapot", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    mainGui.Init(window);

    //Needs to go after makeContextCurrent
    if(!gladLoadGL()){
        printf("Something went wrong with loading GLAD!\n");
        glfwTerminate();
        return -1;
    }
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

    registerInputs(window);

    glEnable(GL_DEPTH_TEST);

    initSceneUniformBlock();

    //MATERIALS
    auto* skyboxTexture = MaterialInterface::LoadCubeMapTexture("../assets/cubemap/cubemap");

    //Depth Material for Shadow Mapping
    auto* depthMat = new DepthMaterial("../shaders/depthShader.vert", "../shaders/depthShader.frag");

    directional_light_shadow_map = new DepthFrameBuffer(1024, 1024);

    //Gizmo Material
    auto* gizmoMat = new StandardMaterial("../shaders/lit.vert", "../shaders/gizmo.frag");
    gizmoMat->diffuseColor = glm::vec3(0.6f, 0.6f, 0.3f);
    gizmoMat->sceneBlockUBO = scene.scene_ubo_id;
    gizmoMat->castShadow = false;

    //Standard Lit Material
    auto* woodTexture = MaterialInterface::LoadTexture("../assets/wood.jpg");
    auto* cobbleSpecTexture = MaterialInterface::LoadTexture("../assets/cobble-specular.png");
    auto* normalTexture = MaterialInterface::LoadTexture("../assets/teapot_normals.png");
    auto* standardMat = new StandardMaterial("../shaders/lit.vert", "../shaders/lit.frag");
    standardMat->diffuseTextID = woodTexture->textureID;
    standardMat->specularTextID = cobbleSpecTexture->textureID;
    standardMat->normalTextID = normalTexture->textureID;
    standardMat->cubemapID = skyboxTexture->textureID;
    standardMat->ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
    standardMat->diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    standardMat->sceneBlockUBO = scene.scene_ubo_id;

    //Skybox Material
    auto* skyboxMat = new SkyboxMaterial("../shaders/skybox.vert", "../shaders/skybox.frag");
    skyboxMat->cubemapID = skyboxTexture->textureID;

    //RenderTexture Material
    fb = new FrameBuffer(1024, 1024);
    auto* renderTextureMat =  new RenderMaterial("../shaders/lit.vert", "../shaders/unlit.frag");

    //HELMET MATERIAL PBR
    auto* helmetPBRMaterial = new PBRMaterial("../shaders/lit.vert", "../shaders/pbr.frag");
    auto* helmetAlbedo = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_Helmet_diffuse.png");
    auto* helmetRoughness = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_Helmet_roughness.png");
    auto* helmetNormal = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_Helmet_normal.png");
    helmetPBRMaterial->diffuseTextID = helmetAlbedo->textureID;
    helmetPBRMaterial->roughnessTextID = helmetRoughness->textureID;
    helmetPBRMaterial->normalTextID = helmetNormal->textureID;
    helmetPBRMaterial->cubemapID = skyboxTexture->textureID;
    helmetPBRMaterial->roughness = 0.1f;
    helmetPBRMaterial->light_intensity = 1.0f;
    helmetPBRMaterial->is_metal = true;
    helmetPBRMaterial->color = glm::vec3(1.0, 1.0, 1.0);

    //HELMET FACE MATERIAL PBR
    auto* helmetFacePBRMaterial = new PBRMaterial("../shaders/lit.vert", "../shaders/pbr.frag");
    auto* helmetFaceAlbedo = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_FaceGuard_diffuse.png");
    auto* helmetFaceRoughness = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_FaceGuard_roughness.png");
    auto* helmetFaceNormal = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_FaceGuard_normal.png");
    helmetFacePBRMaterial->diffuseTextID = helmetFaceAlbedo->textureID;
    helmetFacePBRMaterial->roughnessTextID = helmetFaceRoughness->textureID;
    helmetFacePBRMaterial->normalTextID = helmetFaceNormal->textureID;
    helmetFacePBRMaterial->cubemapID = skyboxTexture->textureID;
    helmetFacePBRMaterial->roughness = 0.1f;
    helmetFacePBRMaterial->light_intensity = 1.0f;
    helmetFacePBRMaterial->is_metal = true;
    helmetFacePBRMaterial->color = glm::vec3(1.0, 1.0, 1.0);

    //HELMET MATERIAL PBR
    auto* helmetNeckPBRMaterial = new PBRMaterial("../shaders/lit.vert", "../shaders/pbr.frag");
    auto* helmetNeckAlbedo = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_NeckGuard_diffuse.png");
    auto* helmetNeckRoughness = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_NeckGuard_roughness.png");
    auto* helmetNeckNormal = MaterialInterface::LoadTexture("../assets/salletHelmet/SalletHelmet_NeckGuard_normal.png");
    helmetNeckPBRMaterial->diffuseTextID = helmetNeckAlbedo->textureID;
    helmetNeckPBRMaterial->roughnessTextID = helmetNeckRoughness->textureID;
    helmetNeckPBRMaterial->normalTextID = helmetNeckNormal->textureID;
    helmetNeckPBRMaterial->cubemapID = skyboxTexture->textureID;
    helmetNeckPBRMaterial->roughness = 0.1f;
    helmetNeckPBRMaterial->light_intensity = 1.0f;
    helmetNeckPBRMaterial->is_metal = true;
    helmetNeckPBRMaterial->color = glm::vec3(1.0, 1.0, 1.0);

    std::vector<PBRMaterial*> helmetMaterials;
    helmetMaterials.push_back(helmetPBRMaterial);
    helmetMaterials.push_back(helmetFacePBRMaterial);
    helmetMaterials.push_back(helmetNeckPBRMaterial);

    //GAME OBJECTS

    //Sallet Helmet
    auto* helmetS = new SceneImporter("../assets/salletHelmet/SalletHelmet.fbx");
    for(unsigned int i=0;i<helmetS->meshCollection.size();i++){
        auto* helmet = initGameObject("Helmet_"+std::to_string(i));
        helmet->SetMesh(helmetS->meshCollection[i]);
        helmet->material = helmetMaterials[i];
        helmet->depthMaterial = depthMat;//this is the material used in the shadow depth pass
        helmet->material->shadowMapID = directional_light_shadow_map->texture->textureID;
        scene.backBufferObjects.push_back(helmet);
        scene.selectedGameObject = helmet;
    }


    //LightBulb Game Object
    auto* lightBulbMesh = initMesh("../assets/lightbulb.obj");
    auto* lightBulb = initGameObject("Lightbulb");

    lightBulb->SetMesh(lightBulbMesh);
    lightBulb->material = gizmoMat;
    lightBulb->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    lightBulb->material->shadowMapID = directional_light_shadow_map->texture->textureID;

    scene.directionalLight.gizmoObjTransform = &lightBulb->transform;

    scene.backBufferObjects.push_back(lightBulb);

    //Skybox Game Object
    auto* cube = initMesh("../assets/cube.obj");
    auto* skybox = initGameObject("Skybox");
    skybox->SetMesh(cube);
    skybox->material = skyboxMat;
    skyboxBufferObjects.push_back(skybox);

    //Floor Game Object
//    auto* planeMesh = initMesh("../assets/plane.obj");
//    auto* floor = initGameObject();
//    floor->mesh = planeMesh;
//    floor->material = standardMat;
//    floor->depthMaterial = depthMat;//this is the material used in the shadow depth pass
//    floor->material->shadowMapID = directional_light_shadow_map->texture->textureID;
//    backBufferObjects.push_back(floor);

    //Shadow Mapping Debug
    auto* shadowMapTextureMat = new RenderMaterial("../shaders/lit.vert", "../shaders/unlit.frag");

    auto* debugMesh = initMesh("../assets/plane.obj");
    auto* debug = initGameObject("DebugObj");
    debug->transform.AddPosition(glm::vec3(40.0f,0.0f, 0.0f));
    debug->transform.AddRotation(glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
    debug->SetMesh(debugMesh);
    debug->material = shadowMapTextureMat;
    debug->material->renderedTextID = directional_light_shadow_map->texture->textureID;
    debug->depthMaterial = depthMat;
    scene.backBufferObjects.push_back(debug);

    //Render Texture Debug
    auto* renderTextureMesh = initMesh("../assets/plane.obj");
    auto* renderTextureObj = initGameObject("RenderObj");
    renderTextureObj->transform.AddPosition(glm::vec3(80.0f, 0.0f, 0.0f));
    renderTextureObj->transform.AddRotation(glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
    renderTextureObj->SetMesh(renderTextureMesh);
    renderTextureObj->material = renderTextureMat;
    renderTextureObj->material->renderedTextID = fb->texture->textureID;
    renderTextureObj->depthMaterial = depthMat;
    scene.backBufferObjects.push_back(renderTextureObj);

    scene.camera.SetCameraPos(glm::vec3(0., -2., 60.));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(throw_exit){
            std::cout << "Exit by throw_exit" << std::endl;
            glfwTerminate();
            return 0;
        }
        draw();
    }

    mainGui.Close();

    glfwTerminate();
    return 0;
}