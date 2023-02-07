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
#include "../renderer/Mesh.h"
#include "../renderer/Material.h"

GLFWwindow* window;
static bool throw_exit = false;
static double timer = 0;
unsigned int vao;
int width = 640;
int height = 480;

GLuint programID;

glm::vec3 camPosition = glm::vec3(0,0,0);
glm::vec2 camRotation = glm::vec2(0,0);

bool lMouseBtn = false;
bool lMouseBtnCntrl = false;
double xMousePos = 0;
double yMousePos = 0;

Mesh parsedMesh;

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

void setProjection(glm::vec2 rotation, glm::vec3 translation){
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);

    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    glm::mat4 MVP = Projection * ViewRotateY * Model;

    GLuint mvp_location = glGetUniformLocation(programID, "mvp");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
}

void initializeProgram(){
    Material parsedMaterial = Material(parsedMesh);
    programID = parsedMaterial.programID;
}

void initializeMeshWithAssimp(){
    const char* path = "../assets/teapot.obj";
    parsedMesh = MeshParser::Process(path);

    std::cout << "Done Parsing Mesh " << std::endl;
    std::cout << parsedMesh.TextureCoords.data() << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertexBuffer, normalBuffer, textureCoordBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);
    glGenBuffers(1, &textureCoordBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, parsedMesh.Vertices.size() * sizeof(float), parsedMesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, parsedMesh.Normals.size() * sizeof(float), parsedMesh.Normals.data(), GL_STATIC_DRAW);

    //Textures
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, parsedMesh.TextureCoords.size() * sizeof(float), parsedMesh.TextureCoords.data(), GL_STATIC_DRAW);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, parsedMesh.Indices.size() * sizeof(unsigned int), parsedMesh.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void draw(){
    timer = glfwGetTime();

    GLint time_location = glGetUniformLocation(programID, "timer");
    glUniform1f(time_location, (float)timer);

    GLint directional_light_location = glGetUniformLocation(programID, "directionalLight");
    glUniform3fv(directional_light_location, 1, &directional_light.direction[0]);

    setProjection(camRotation, camPosition);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render here */
    glDrawElementsInstanced(GL_TRIANGLES, parsedMesh.Indices.size(), GL_UNSIGNED_INT, 0, 2);
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
    initializeMeshWithAssimp();
    initializeProgram();


    onMoveCamera(glm::vec3(0., 0., -20.));
    setProjection(camRotation, camPosition);

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