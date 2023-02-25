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

GLFWwindow* window;
static bool throw_exit = false;
int width = 640;
int height = 480;

GLuint frameBufferID;

glm::vec3 camPosition = glm::vec3(0,0,0);
glm::vec2 camRotation = glm::vec2(0,0);

bool lMouseBtn = false;
bool lMouseBtnCntrl = false;
double xMousePos = 0;
double yMousePos = 0;

std::vector<GameObject*> frameBufferObjects;
std::vector<GameObject*> backBufferObjects;
std::vector<GameObject*> skyboxBufferObjects;

void onMoveCamera(glm::vec3 translation){
    camPosition += translation;
}

void onCursorPosition(glm::vec2 position)
{
    if(lMouseBtn){
        camRotation.x += (position.x - xMousePos) * 0.01f;
        camRotation.y += (position.y - yMousePos) * 0.01f;
    }
    if(lMouseBtnCntrl){
        Scene::directional_light.direction.x += (position.x - xMousePos) * 0.01f;
        Scene::directional_light.direction.y -= (position.y - yMousePos) * 0.01f;
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
    camPosition.z += scrollOffset.y;
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

glm::mat4 GetCameraProjection(glm::vec2 rotation, glm::vec3 translation){

    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), translation);

    glm::mat4 ViewRotate = glm::rotate(ViewTranslate, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    ViewRotate = glm::rotate(ViewRotate, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

    return ViewRotate;
}

Mesh* InitMesh(const std::string& path){
    auto* mesh = new Mesh();
    mesh->Parse(path.c_str());
    mesh->InitBuffers();
    return mesh;
}

GameObject* InitGameObject(const std::string& meshPath){
    auto* go = new GameObject();
//    go->transform.position = glm::linearRand(glm::vec3(-50), glm::vec3(50));
    go->mesh = InitMesh(meshPath);
    return go;
}

void InitStandardTexture(GameObject* gObj, aiTextureType type, GLenum textureLocation, unsigned int &id){
    //TODO - This is a hack, I need to fix this - can be deleted after assignment etc.
    Texture texture = MaterialInterface::LoadTexture(type, gObj->mesh->meshMaterialData);

    //Assign diffuseTexture
    glActiveTexture(textureLocation);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // all upcoming GL_TEXTURE_2D operations now have effect on this diffuseTexture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//generate bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//What to do with outside coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void InitCubeMapTexture(GameObject* gObj){
    GLuint cubemapTextureID;
    glGenTextures(1, &cubemapTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

    int width, height, nrChannels;
    unsigned char *data;

// Load positive X
    data = stbi_load("../assets/cubemap/cubemap_posx.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative X
    data = stbi_load("../assets/cubemap/cubemap_negx.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load positive Y
    data = stbi_load("../assets/cubemap/cubemap_posy.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative Y
    data = stbi_load("../assets/cubemap/cubemap_negy.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load positive Z
    data = stbi_load("../assets/cubemap/cubemap_posz.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative Z
    data = stbi_load("../assets/cubemap/cubemap_negz.png", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::cout << "Cubemap Texture ID: " << cubemapTextureID << std::endl;

    gObj->material->cubemapID = cubemapTextureID;
}

void InitProgramAsStandard(GameObject* gameObj, std::string vertex_path, std::string fragment_path){
    gameObj->material = new StandardMaterial(std::move(vertex_path), std::move(fragment_path));
    InitStandardTexture(gameObj, aiTextureType_DIFFUSE, GL_TEXTURE0, gameObj->material->diffuseID);
    InitStandardTexture(gameObj, aiTextureType_SPECULAR, GL_TEXTURE1, gameObj->material->specularID);
}

void InitProgramAsRender(GameObject* gameObj, std::string vertex_path, std::string fragment_path){
    gameObj->material = new RenderMaterial(std::move(vertex_path), std::move(fragment_path));
    InitRenderTexture(gameObj);
}

void InitProgramAsSkybox(GameObject* gameObj, std::string vertex_path, std::string fragment_path){
    gameObj->material = new SkyboxMaterial(std::move(vertex_path), std::move(fragment_path));
    InitCubeMapTexture(gameObj);
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
    Scene::ViewMatrix = GetCameraProjection(camRotation, camPosition);

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

    glEnable (GL_DEPTH_TEST);

    //I need a parsedMesh to get the materials, so order matters here

    auto* suzanne = InitGameObject("../assets/suzanne.obj");
    InitProgramAsStandard(suzanne, "../shaders/lit.vert", "../shaders/lit.frag");
    backBufferObjects.push_back(suzanne);

//    auto* teapot = InitGameObject("../assets/teapot.obj");
//    InitProgramAsStandard(teapot, "../shaders/lit.vert", "../shaders/lit.frag");
//    backBufferObjects.push_back(teapot);

    auto* plane = InitGameObject("../assets/plane.obj");
    InitProgramAsSkybox(plane, "../shaders/skybox.vert", "../shaders/skybox.frag");
    skyboxBufferObjects.push_back(plane);

    onMoveCamera(glm::vec3(0., 0., -40.));
    //camRotation.y = 1.0f;

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