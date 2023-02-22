#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include "MeshParser.h"
#include "InputInterface.h"
#include "../renderer/DirectionalLight.h"
#include "MaterialInterface.h"
#include "../renderer/Material.h"
#include "../renderer/StandardMaterial.h"
#include "../renderer/RenderMaterial.h"

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

std::vector<Mesh> frameBufferMeshes;
std::vector<Mesh> backBufferMeshes;

DirectionalLight directional_light;

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
        std::cout << "Mods mouse button";
        directional_light.direction.x += (position.x - xMousePos) * 0.01f;
        directional_light.direction.y -= (position.y - yMousePos) * 0.01f;
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

glm::mat4 GetProjection(glm::vec2 rotation, glm::vec3 translation){
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    glm::mat4 MVP = Projection * ViewRotateY * Model;

    return MVP;
}

void InitMesh(std::string path, std::vector<Mesh> &meshList){
    Mesh mesh = MeshParser::Process(path.c_str());

    std::cout << "Done Parsing Mesh " << std::endl;
    std::cout << mesh.TextureCoords.data() << std::endl;

    glGenVertexArrays(1, &mesh.vaoID);
    glBindVertexArray(mesh.vaoID);

    GLuint vertexBuffer, normalBuffer, textureCoordBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &textureCoordBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    std::cout << "Index of Vertex Buffer" << vertexBuffer << std::endl;
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(float), mesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.Normals.size() * sizeof(float), mesh.Normals.data(), GL_STATIC_DRAW);

    //Textures
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.TextureCoords.size() * sizeof(float), mesh.TextureCoords.data(), GL_STATIC_DRAW);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(unsigned int), mesh.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    meshList.push_back(mesh);
    std::cout << "Loaded " << meshList.size() << " meshes" << std::endl;

}

void InitStandardTexture(Mesh &mesh, aiTextureType type, GLenum textureLocation, unsigned int &id){
    //TODO - This is a hack, I need to fix this - can be deleted after assignment etc.
    Texture texture = MaterialInterface::LoadTexture(type, mesh.meshMaterialData);

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


void InitProgramAsStandard(Mesh &mesh, std::string vertex_path, std::string fragment_path){
    mesh.material = new StandardMaterial(vertex_path, fragment_path);
}

void InitProgramAsRender(Mesh &mesh, std::string vertex_path, std::string fragment_path){
    mesh.material = new RenderMaterial(vertex_path, fragment_path);
}

void InitRenderTexture(Mesh &mesh){
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

    mesh.material->renderedTextureID = renderedTextureID;
}

void drawFrameBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
    glViewport(0,0,1024,1024);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    unsigned int length = frameBufferMeshes.size();

    glm::mat4 MVP = GetProjection(camRotation, camPosition);
    for(unsigned int i = 0; i < length; i++){
        Mesh* mesh = &frameBufferMeshes[i];
        mesh->material->Draw();

        GLint directional_light_location = glGetUniformLocation(mesh->material->programID, "directionalLight");
        glUniform3fv(directional_light_location, 1, &directional_light.direction[0]);

        glBindVertexArray(frameBufferMeshes[i].vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, mesh->Indices.size(), GL_UNSIGNED_INT, 0, 2);

        GLuint mvp_location = glGetUniformLocation(mesh->material->programID, "mvp");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
    }
    //Regerate MipMap Levels for render texture
}

void drawBackBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 MVP = GetProjection(camRotation, camPosition);
    for(unsigned int i = 0; i < backBufferMeshes.size(); i++){

        Mesh mesh = backBufferMeshes[i];
        mesh.material->Draw();

        glBindVertexArray(mesh.vaoID);
        glDrawElementsInstanced(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0, 2);

        GLuint mvp_location = glGetUniformLocation(mesh.material->programID, "mvp");
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
    }
}

void draw(){
    drawFrameBuffer();
    drawBackBuffer();

    /* End of Render */

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

    /* TODO Initialize Buffer Objects */
    directional_light.intensity = 1.;
    directional_light.direction = glm::vec3(0., 0., -1.);

    //I need a parsedMesh to get the materials, so order matters here

    InitMesh("../assets/suzanne.obj", frameBufferMeshes);
    InitProgramAsStandard(frameBufferMeshes[0], "../shaders/lit.vert", "../shaders/lit.frag");
    InitStandardTexture(frameBufferMeshes[0], aiTextureType_DIFFUSE, GL_TEXTURE0, frameBufferMeshes[0].material->diffuseID);
    InitStandardTexture(frameBufferMeshes[0], aiTextureType_SPECULAR, GL_TEXTURE1, frameBufferMeshes[0].material->specularID);

    InitMesh("../assets/teapot.obj", frameBufferMeshes);
    InitProgramAsStandard(frameBufferMeshes[1], "../shaders/lit.vert", "../shaders/lit.frag");
    InitStandardTexture(frameBufferMeshes[1], aiTextureType_DIFFUSE, GL_TEXTURE0, frameBufferMeshes[1].material->diffuseID);
    InitStandardTexture(frameBufferMeshes[1], aiTextureType_SPECULAR, GL_TEXTURE1, frameBufferMeshes[1].material->specularID);

    InitMesh("../assets/plane.obj", backBufferMeshes);
    InitProgramAsRender(backBufferMeshes[0], "../shaders/lit.vert", "../shaders/unlit.frag");
    InitRenderTexture(backBufferMeshes[0]);

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