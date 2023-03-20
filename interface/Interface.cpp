#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <utility>
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
#include "../renderer/FrameBuffer.h"
#include "../renderer/DepthFrameBuffer.h"
#include "imgui.h"
#include "../libraries/imgui/backends/imgui_impl_glfw.h"
#include "../libraries/imgui/backends/imgui_impl_opengl3.h"
#include "../renderer/LightUniformBlock.h"

GLFWwindow* window;
static bool throw_exit = false;
int width = 1024;
int height = 768;

bool lMouseBtn = false;
bool lMouseBtnCntrl = false;
double xMousePos = 0;
double yMousePos = 0;

FrameBuffer* fb;
DepthFrameBuffer* directional_light_shadow_map;

GLuint light_ubo;

std::vector<GameObject*> backBufferObjects = std::vector<GameObject*>();
std::vector<GameObject*> skyboxBufferObjects = std::vector<GameObject*>();

void onMoveCamera(glm::vec3 translation){
    Scene::CameraTransform.position += translation;
}

void onCursorPosition(glm::vec2 position)
{
    if(lMouseBtn){
        Scene::CameraTransform.rotation.x += (position.x - xMousePos) * 0.01f;
        Scene::CameraTransform.rotation.y += (position.y - yMousePos) * 0.01f;
    }
    if(lMouseBtnCntrl){
        Scene::directional_light.direction.x += (position.x - xMousePos) * 0.1f;
        Scene::directional_light.direction.y -= (position.y - yMousePos) * 0.1f;
    }
    xMousePos = position.x;
    yMousePos = position.y;
}

void onMouseButtonCallback(int button, int action, int mods)
{
    lMouseBtn = (action == GLFW_PRESS && button == 0 && mods == 0);
    lMouseBtnCntrl = (action == GLFW_PRESS && button == 0 && (mods & GLFW_MOD_CONTROL) != 0);
}

void onScrollCallback(glm::vec2 scrollOffset)
{
    Scene::CameraTransform.position.z += scrollOffset.y;
}

void registerInputs(GLFWwindow* window){
    auto input = new InputInterface(window, onCursorPosition, onMouseButtonCallback, onScrollCallback);
    input->Subscribe(GLFW_KEY_A, [](){ onMoveCamera(glm::vec3(0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_D, [](){ onMoveCamera(glm::vec3(-0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_W, [](){ onMoveCamera(glm::vec3(0., -0.1, 0.));});
    input->Subscribe(GLFW_KEY_S, [](){ onMoveCamera(glm::vec3(0., 0.1, 0.));});
    input->Subscribe(GLFW_KEY_ESCAPE, [=](){throw_exit = true;});
    input->InitKeyCallback();
}

glm::mat4 GetProjection(){
    return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.f);
}

glm::mat4 GetCameraProjection(Transform cameraTransform){
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), cameraTransform.position);
    glm::mat4 ViewRotate = glm::rotate(ViewTranslate, cameraTransform.rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    ViewRotate = glm::rotate(ViewRotate, cameraTransform.rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    return ViewRotate;
}

Mesh* InitMesh(const std::string& path){
    auto* mesh = new Mesh();
    mesh->Parse(path.c_str());
    mesh->InitBuffers();
    return mesh;
}

GameObject* InitGameObject(){
    auto* go = new GameObject();
//    go->transform.position = glm::linearRand(glm::vec3(-50), glm::vec3(50));
    return go;
}

void InitLightUniformBlock(){
    glGenBuffers(1, &light_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUniformBlock), NULL, GL_DYNAMIC_DRAW);

    Scene::LightBlock.lightColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUniformBlock), &Scene::LightBlock);
}

void drawFrameBuffer(FrameBuffer* buffer){
    glBindFramebuffer(GL_FRAMEBUFFER, buffer->id);
    glViewport(0,0,buffer->texture->width,buffer->texture->width);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto gObj : backBufferObjects){

        gObj->Draw();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
    //Regerate MipMap Levels for render texture
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);

    for(auto gObj : backBufferObjects){

        gObj->Draw();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
}

void drawShadowBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, directional_light_shadow_map->id);
    glViewport(0, 0, directional_light_shadow_map->texture->width, directional_light_shadow_map->texture->width);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(auto gObj : backBufferObjects){

        gObj->DrawDepth();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }

}

void drawSkyboxBuffer(){
    for(auto gObj : skyboxBufferObjects){

        gObj->Draw();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
}

void draw(){
    Scene::ProjectionMatrix = GetProjection();
    Scene::ViewMatrix = GetCameraProjection(Scene::CameraTransform);

    Scene::LightBlock.lightColor = glm::vec4(1.0, 0.0, 0.0, 0.0);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUniformBlock), &Scene::LightBlock);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Hello, teapot!");
    ImGui::Text("Hello, world!");
    if(ImGui::Button("Button")){
        std::cout << "Miaf!";
    }
    ImGui::End();

    // Render the ImGui elements to the screen
    ImGui::Render();

    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthMask(GL_FALSE);
    drawSkyboxBuffer();
    glDepthMask(GL_TRUE);
    drawShadowBuffer();
    drawFrameBuffer(fb);
    drawBackBuffer();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    //handle interface events
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int run() {
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
    ImGuiIO io = ImGui::GetIO(); (void)io;

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

    InitLightUniformBlock();

    auto* skyboxTexture = MaterialInterface::LoadCubeMapTexture("../assets/cubemap/cubemap");

    //Depth Material for Shadow Mapping
    auto* depthMat = new DepthMaterial("../shaders/depthShader.vert", "../shaders/depthShader.frag");

    directional_light_shadow_map = new DepthFrameBuffer(1024, 1024);

    //Standard Lit Material
    auto* woodTexture =  MaterialInterface::LoadTexture("../assets/wood.jpg");
    auto* cobbleSpecTexture = MaterialInterface::LoadTexture("../assets/cobble-specular.png");
    auto* standardMat = new StandardMaterial("../shaders/lit.vert", "../shaders/lit.frag");
    standardMat->diffuseID = woodTexture->textureID;
    standardMat->specularID = cobbleSpecTexture->textureID;
    standardMat->cubemapID = skyboxTexture->textureID;
    standardMat->diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    standardMat->lightBlockUBO = light_ubo;

    //Skybox Material
    auto* skyboxMat = new SkyboxMaterial("../shaders/skybox.vert", "../shaders/skybox.frag");
    skyboxMat->cubemapID = skyboxTexture->textureID;

    //Teapot Game Object
    auto* teapotMesh = InitMesh("../assets/teapot.obj");
    auto* teapot = InitGameObject();
    teapot->mesh = teapotMesh;
    teapot->material = standardMat;
    teapot->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    teapot->material->shadowMapID = directional_light_shadow_map->texture->textureID;
    backBufferObjects.push_back(teapot);

    //Skybox Game Object
    auto* cube = InitMesh("../assets/cube.obj");
    auto* skybox = InitGameObject();
    skybox->material = skyboxMat;
    skybox->mesh = cube;
    skyboxBufferObjects.push_back(skybox);

    //Floor Game Object
    auto* planeMesh = InitMesh("../assets/plane.obj");
    auto* floor = InitGameObject();
    floor->mesh = planeMesh;
    floor->material = standardMat;
    floor->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    floor->material->shadowMapID = directional_light_shadow_map->texture->textureID;
    backBufferObjects.push_back(floor);

    //Shadow Mapping Debug
    auto* shadowMapTextureMat = new RenderMaterial("../shaders/lit.vert", "../shaders/unlit.frag");

    auto* debugMesh = InitMesh("../assets/plane.obj");
    auto* debug = InitGameObject();
    debug->transform.position.x += 40.0f;
    debug->transform.rotation.x += M_PI / 2.0f;
    debug->mesh = debugMesh;
    debug->material = shadowMapTextureMat;
    debug->material->renderedTextureID = directional_light_shadow_map->texture->textureID;
    debug->depthMaterial = depthMat;
    backBufferObjects.push_back(debug);

    //Render Texture Material
    fb = new FrameBuffer(1024, 1024);

    auto* renderTextureMat =  new RenderMaterial("../shaders/lit.vert", "../shaders/unlit.frag");

    auto* renderTextureMesh = InitMesh("../assets/plane.obj");
    auto* renderTextureObj = InitGameObject();
    renderTextureObj->transform.position.x += 80.0f;
    renderTextureObj->transform.rotation.x += M_PI / 2.0f;
    renderTextureObj->mesh = renderTextureMesh;
    renderTextureObj->material = renderTextureMat;
    renderTextureObj->material->renderedTextureID = fb->texture->textureID;
    renderTextureObj->depthMaterial = depthMat;
    backBufferObjects.push_back(renderTextureObj);

    onMoveCamera(glm::vec3(0., -2., -60.));

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}