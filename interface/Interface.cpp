#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include "MeshParser.h"
#include "InputInterface.h"
#include "../renderer/DirectionalLight.h"
#include "MaterialInterface.h"
#include "../renderer/Materials/Material.h"
#include "../renderer/Materials/StandardMaterial.h"
#include "../renderer/Materials/RenderMaterial.h"
#include "../renderer/GameObject.h"
#include "../renderer/Materials/SkyboxMaterial.h"
#include "../renderer/Materials/DepthMaterial.h"

GLFWwindow* window;
static bool throw_exit = false;
int width = 640;
int height = 480;

GLuint frameBufferID;

bool lMouseBtn = false;
bool lMouseBtnCntrl = false;
double xMousePos = 0;
double yMousePos = 0;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int shadowMapID;


std::vector<GameObject*> frameBufferObjects = std::vector<GameObject*>();
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

void InitRenderTexture(GameObject* gObj){
    GLuint renderedTextureID;

    int textureWidth = 1024;
    int textureHeight = 1024;

    glGenFramebuffers(1, &frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

    glGenTextures(1, &renderedTextureID);
    glBindTexture(GL_TEXTURE_2D, renderedTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textureWidth, textureHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTextureID, 0);
    GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);

    //Wait until done
    bool checking = true;
    while(checking){
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE){
            checking = false;
        }
    }

    gObj->material->renderedTextureID = renderedTextureID;
}

void InitShadowMapTexture(){
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &shadowMapID);
    glBindTexture(GL_TEXTURE_2D, shadowMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

Material* InitProgramAsStandard(std::string vertex_path, std::string fragment_path){
    return new StandardMaterial(std::move(vertex_path), std::move(fragment_path));
}

Material* InitProgramAsRender(std::string vertex_path, std::string fragment_path){
    return new RenderMaterial(std::move(vertex_path), std::move(fragment_path));
}

Material* InitProgramAsSkybox(std::string vertex_path, std::string fragment_path){
    return new SkyboxMaterial(std::move(vertex_path), std::move(fragment_path));
}

Material* InitProgramAsDepth(std::string vertex_path, std::string fragment_path){
    return new DepthMaterial(std::move(vertex_path), std::move(fragment_path));
}

void drawFrameBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glViewport(0,0,1024,1024);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unsigned int length = frameBufferObjects.size();

    for(unsigned int i = 0; i < length; i++){
        GameObject* gObj = frameBufferObjects[i];
        gObj->Draw();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
    //Regerate MipMap Levels for render texture
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
//    glClearColor(.0f, .0f, .0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < backBufferObjects.size(); i++){

        GameObject* gObj = backBufferObjects[i];
        gObj->Draw();

        std::cout << "Indices Count: " << gObj->mesh->Indices.size() << std::endl;

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
}

void drawShadowBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < backBufferObjects.size(); i++){

        GameObject* gObj = backBufferObjects[i];
        gObj->DrawDepth();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }

}

void drawSkyboxBuffer(){
    for(unsigned int i = 0; i < skyboxBufferObjects.size(); i++){

        GameObject* gObj = skyboxBufferObjects[i];
        gObj->Draw();

        glBindVertexArray(gObj->mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, gObj->mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
}

void draw(){
    Scene::ProjectionMatrix = GetProjection();
    Scene::ViewMatrix = GetCameraProjection(Scene::CameraTransform);

//    backBufferObjects[0]->transform.position -= glm::vec3(0.0f, 0.1f, 0.0f);
//    backBufferObjects[0]->transform.scale -= glm::vec3(0.001f, 0.001f, 0.001f);
//    backBufferObjects[0]->transform.rotation -= glm::vec3(0.0f, 0.1f, 0.0f);

//    drawFrameBuffer();
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Not the most optimal way but good for now
    glDepthMask(GL_FALSE);
    drawSkyboxBuffer();
    glDepthMask(GL_TRUE);
    drawShadowBuffer();
    drawBackBuffer();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    //handle interface events
    glfwPollEvents();
}

int run() {
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

    //Needs to go after makeContextCurrent
    if(!gladLoadGL()){
        printf("Something went wrong with loading GLAD!\n");
        glfwTerminate();
        return -1;
    }
    printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    registerInputs(window);

    glEnable(GL_DEPTH_TEST);

    auto* skyboxTexture = MaterialInterface::LoadCubeMapTexture("../assets/cubemap/cubemap");

    //Depth Material for Shadow Mapping
    auto* depthMat = InitProgramAsDepth("../shaders/depthShader.vert", "../shaders/depthShader.frag");
    InitShadowMapTexture();

    //Standard Lit Material
    auto* woodTexture =  MaterialInterface::LoadTexture("../assets/wood.jpg");
    auto* cobbleSpecTexture = MaterialInterface::LoadTexture("../assets/cobble-specular.png");
    auto* standardMat = InitProgramAsStandard("../shaders/lit.vert", "../shaders/lit.frag");
    standardMat->diffuseID = woodTexture->textureID;
    standardMat->specularID = cobbleSpecTexture->textureID;
    standardMat->cubemapID = skyboxTexture->textureID;
    standardMat->diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);

    //Render Texture Material
    auto* renderTxt = InitProgramAsRender("../shaders/lit.vert", "../shaders/unlit.frag");

    //Skybox Material
    auto* skyboxMat = InitProgramAsSkybox("../shaders/skybox.vert", "../shaders/skybox.frag");
    skyboxMat->cubemapID = skyboxTexture->textureID;

    //Teapot Game Object
    auto* teapotMesh = InitMesh("../assets/teapot.obj");
    auto* teapot = InitGameObject();
    teapot->mesh = teapotMesh;
    teapot->material = standardMat;
    teapot->depthMaterial = depthMat;//this is the material used in the shadow depth pass
    teapot->material->shadowMapID = shadowMapID;
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
    floor->material->shadowMapID = shadowMapID;
    backBufferObjects.push_back(floor);

    //Shadow Mapping Debug
    auto* debugMesh = InitMesh("../assets/plane.obj");
    auto* debug = InitGameObject();
    debug->transform.position.x += 40.0f;
    debug->transform.rotation.x += M_PI / 2.0f;
    debug->mesh = debugMesh;
    debug->material = renderTxt;
    debug->material->renderedTextureID = shadowMapID;
    debug->depthMaterial = depthMat;
    backBufferObjects.push_back(debug);

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

    glfwTerminate();
    return 0;
}