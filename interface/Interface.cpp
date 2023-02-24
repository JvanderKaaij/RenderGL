#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include "glm/ext.hpp"
#include "MeshParser.h"
#include "InputInterface.h"
#include "../renderer/DirectionalLight.h"
#include "MaterialInterface.h"
#include "../renderer/Material.h"
#include "../renderer/StandardMaterial.h"
#include "../renderer/RenderMaterial.h"
#include "../renderer/Scene.h"
#include "../renderer/GameObject.h"

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

std::vector<GameObject*> frameBufferMeshes;
std::vector<GameObject*> backBufferMeshes;

std::vector<GameObject> gameObjects;

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

glm::mat4 GetCameraProjection(glm::vec2 rotation, glm::vec3 translation){
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    glm::mat4 MVP = Projection * ViewRotateY * Model;

    return MVP;
}

Mesh* InitMesh(const std::string& path){
    auto* mesh = new Mesh();
    mesh->Parse(path.c_str());
    mesh->InitBuffers();
    return mesh;
}

GameObject* InitGameObject(const std::string& meshPath){
    auto* go = new GameObject();
    go->mesh = InitMesh(meshPath);
    return go;
}

void InitStandardTexture(Mesh* mesh, aiTextureType type, GLenum textureLocation, unsigned int &id){
    //TODO - This is a hack, I need to fix this - can be deleted after assignment etc.
    Texture texture = MaterialInterface::LoadTexture(type, mesh->meshMaterialData);

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


void InitProgramAsStandard(Mesh* mesh, std::string vertex_path, std::string fragment_path){
    mesh->material = new StandardMaterial(vertex_path, fragment_path);
}

void InitProgramAsRender(Mesh* mesh, std::string vertex_path, std::string fragment_path){
    mesh->material = new RenderMaterial(vertex_path, fragment_path);
}

void InitRenderTexture(Mesh* mesh){
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

    mesh->material->renderedTextureID = renderedTextureID;
}

void drawFrameBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glViewport(0,0,1024,1024);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unsigned int length = frameBufferMeshes.size();

    for(unsigned int i = 0; i < length; i++){
        Mesh* mesh = frameBufferMeshes[i]->mesh;
        mesh->material->Draw();

        glBindVertexArray(mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
    //Regerate MipMap Levels for render texture
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < backBufferMeshes.size(); i++){

        Mesh* mesh = backBufferMeshes[i]->mesh;
        mesh->material->Draw();

        glBindVertexArray(mesh->vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);
    }
}

void draw(){
    Scene::CameraMatrix = GetCameraProjection(camRotation, camPosition);
    drawFrameBuffer();
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
    InitProgramAsStandard(suzanne->mesh, "../shaders/lit.vert", "../shaders/lit.frag");
    InitStandardTexture(suzanne->mesh, aiTextureType_DIFFUSE, GL_TEXTURE0, suzanne->mesh->material->diffuseID);
    InitStandardTexture(suzanne->mesh, aiTextureType_SPECULAR, GL_TEXTURE1, suzanne->mesh->material->specularID);
    frameBufferMeshes.push_back(suzanne);

    auto* teapot = InitGameObject("../assets/teapot.obj");
    InitProgramAsStandard(teapot->mesh, "../shaders/lit.vert", "../shaders/lit.frag");
    InitStandardTexture(teapot->mesh, aiTextureType_DIFFUSE, GL_TEXTURE0, teapot->mesh->material->diffuseID);
    InitStandardTexture(teapot->mesh, aiTextureType_SPECULAR, GL_TEXTURE1, teapot->mesh->material->specularID);
    frameBufferMeshes.push_back(teapot);

    auto* plane = InitGameObject("../assets/plane.obj");
    InitProgramAsRender(plane->mesh, "../shaders/lit.vert", "../shaders/unlit.frag");
    InitRenderTexture(plane->mesh);
    backBufferMeshes.push_back(plane);

    onMoveCamera(glm::vec3(0., 0., -20.));
    camRotation.y = 1.0f;

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