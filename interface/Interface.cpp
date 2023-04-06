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
#include "../renderer/GameObject.h"
#include "../renderer/Materials/SkyboxMaterial.h"
#include "../renderer/Materials/DepthMaterial.h"
#include "../renderer/Buffers/FrameBuffer.h"
#include "../renderer/Buffers/DepthFrameBuffer.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "../libraries/imgui/backends/imgui_impl_glfw.h"
#include "../libraries/imgui/backends/imgui_impl_opengl3.h"
#include "../renderer/Materials/PBRMaterial.h"
#include <glm/gtx/euler_angles.hpp>

GLFWwindow* window;
static bool throw_exit = false;
int width = 1024;
int height = 768;

float deltaTime = 0.0f;
float lastTime = 0.0f;

FrameBuffer* fb;
DepthFrameBuffer* directional_light_shadow_map;

std::vector<GameObject*> backBufferObjects = std::vector<GameObject*>();
std::vector<GameObject*> skyboxBufferObjects = std::vector<GameObject*>();

Scene scene = Scene();
InputInterface* inputInterface;

GameObject* teapot;

bool rotateTool;
bool translateTool = true;

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

GameObject* initGameObject(){
    auto* go = new GameObject();
//    go->transform.position = glm::linearRand(glm::vec3(-50), glm::vec3(50));
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

    for(auto gObj : backBufferObjects){
        gObj->Draw(scene.sceneUniforms);
    }
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);

    for(auto gObj : backBufferObjects){
        gObj->Draw(scene.sceneUniforms);
    }
}

void drawShadowBuffer(){

    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, directional_light_shadow_map->id);
    glViewport(0, 0, directional_light_shadow_map->texture->width, directional_light_shadow_map->texture->width);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(auto gObj : backBufferObjects){
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

void drawUI(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuizmo::BeginFrame();

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screenSize = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
    ImGui::SetNextWindowSize(screenSize);
    ImGui::SetNextWindowPos(ImVec2(0,0));

    ImGui::Begin("Hello, teapot!", nullptr, ImGuiWindowFlags_NoBackground);

    ImGuizmo::SetOrthographic(false); // Set the projection matrix to perspective
    ImGuizmo::SetDrawlist(); // Setup draw list for rendering
    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),ImGui::GetWindowHeight());
//    ImGuizmo::DrawGrid(glm::value_ptr(scene.camera.GetViewMatrix()), glm::value_ptr(scene.camera.GetProjectionMatrix()), glm::value_ptr(glm::mat4(1.0f)), 100.f);
    if(translateTool){
        ImGuizmo::Manipulate(glm::value_ptr(scene.camera.GetViewMatrix()), glm::value_ptr(scene.camera.GetProjectionMatrix()), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(scene.directionalLight.transform.matrix));
    }
    if(rotateTool){
        ImGuizmo::Manipulate(glm::value_ptr(scene.camera.GetViewMatrix()), glm::value_ptr(scene.camera.GetProjectionMatrix()), ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(scene.directionalLight.transform.matrix));
    }

    if(ImGuizmo::IsUsing()){
    }
    scene.directionalLight.Update();


    ImGui::Text("Hello, teapot!");
    if(ImGui::Button("Translate")){
        rotateTool = false;
        translateTool = true;
    }
    if(ImGui::Button("Rotate")){
        rotateTool = true;
        translateTool = false;
    }
    ImGui::End();

    // Render the ImGui elements to the screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    drawUI();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse) {
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

    window = glfwCreateWindow(width, height, "Hello Teapot", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    ImGui::StyleColorsDark();

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

    //PBR Material
    auto* pbrMaterial = new PBRMaterial("../shaders/lit.vert", "../shaders/pbr.frag");
    auto* teapotAlbedo = MaterialInterface::LoadTexture("../assets/pbr/UtahTeapot_albedo.jpg");
    auto* teapotMetallic = MaterialInterface::LoadTexture("../assets/pbr/UtahTeapot_metallic.jpg");
    auto* teapotRoughness = MaterialInterface::LoadTexture("../assets/pbr/UtahTeapot_roughness.jpg");
    pbrMaterial->diffuseTextID = teapotAlbedo->textureID;
    pbrMaterial->metallicTextID = teapotMetallic->textureID;
    pbrMaterial->roughnessTextID = teapotRoughness->textureID;
    pbrMaterial->roughness = 0.2f;
    pbrMaterial->light_intensity = 3.0f;
    pbrMaterial->is_metal = false;
    pbrMaterial->color = glm::vec3(1.0, 1.0, 1.0);

    //GAME OBJECTS

    //Teapot Game Object
    auto* teapotMesh = initMesh("../assets/pbr/utah_teapot.obj");
    teapot = initGameObject();
    teapot->mesh = teapotMesh;
    teapot->material = pbrMaterial;
    teapot->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    teapot->material->shadowMapID = directional_light_shadow_map->texture->textureID;
    backBufferObjects.push_back(teapot);

    //LightBulb Game Object
    auto* lightBulbMesh = initMesh("../assets/lightbulb.obj");
    auto* lightBulb = initGameObject();

    lightBulb->mesh = lightBulbMesh;
    lightBulb->material = gizmoMat;
    lightBulb->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    lightBulb->material->shadowMapID = directional_light_shadow_map->texture->textureID;

    scene.directionalLight.gizmoObjTransform = &lightBulb->transform;

    backBufferObjects.push_back(lightBulb);

    //Skybox Game Object
    auto* cube = initMesh("../assets/cube.obj");
    auto* skybox = initGameObject();
    skybox->material = skyboxMat;
    skybox->mesh = cube;
    skyboxBufferObjects.push_back(skybox);

    //Floor Game Object
    auto* planeMesh = initMesh("../assets/plane.obj");
    auto* floor = initGameObject();
    floor->mesh = planeMesh;
    floor->material = standardMat;
    floor->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    floor->material->shadowMapID = directional_light_shadow_map->texture->textureID;
    backBufferObjects.push_back(floor);

    //Shadow Mapping Debug
    auto* shadowMapTextureMat = new RenderMaterial("../shaders/lit.vert", "../shaders/unlit.frag");

    auto* debugMesh = initMesh("../assets/plane.obj");
    auto* debug = initGameObject();
    debug->transform.AddPosition(glm::vec3(40.0f,0.0f, 0.0f));
    debug->transform.AddRotation(glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
    debug->mesh = debugMesh;
    debug->material = shadowMapTextureMat;
    debug->material->renderedTextID = directional_light_shadow_map->texture->textureID;
    debug->depthMaterial = depthMat;
    backBufferObjects.push_back(debug);

    //Render Texture Debug
    auto* renderTextureMesh = initMesh("../assets/plane.obj");
    auto* renderTextureObj = initGameObject();
    renderTextureObj->transform.AddPosition(glm::vec3(80.0f, 0.0f, 0.0f));
    renderTextureObj->transform.AddRotation(glm::vec3(M_PI / 2.0f, 0.0f, 0.0f));
    renderTextureObj->mesh = renderTextureMesh;
    renderTextureObj->material = renderTextureMat;
    renderTextureObj->material->renderedTextID = fb->texture->textureID;
    renderTextureObj->depthMaterial = depthMat;
    backBufferObjects.push_back(renderTextureObj);

    scene.camera.SetCameraPos(glm::vec3(0., -2., 60.));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(throw_exit){
            std::cout << "Exit by throw_exit" << std::endl;
            glfwTerminate();
            return 0;
        }
//        teapot->transform.position.x += 0.01f;

//        teapot->transform.rotation.y += 0.05;
//        floor->transform.rotation.y += 0.01;
        draw();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}