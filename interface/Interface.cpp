#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include "InputInterface.h"
#include "MeshParser.h"

static bool throw_exit = false;
static double timer = 0;
unsigned int vao;
int width = 640;
int height = 480;

GLuint programID;

glm::vec3 camPosition = glm::vec3(0,0,0);
glm::vec2 camRotation = glm::vec2(0,0);

bool lMouseBtn = false;
double xMousePos = 0;
double yMousePos = 0;

cy::GLSLShader vert_shader;
cy::GLSLShader frag_shader;
cy::GLSLProgram program;

ParsedMesh parsedMesh;

void OnMoveCamera(glm::vec3 translation){
    camPosition += translation;
}

void onCursorPosition(glm::vec2 position)
{
    if(lMouseBtn){
        camRotation.x += (position.x - xMousePos) * 0.01f;
        camRotation.y += (position.y - yMousePos) * 0.01f;
    }
    xMousePos = position.x;
    yMousePos = position.y;
}

void OnMouseButtonCallback(int button, int action, int mods)
{
    lMouseBtn = (action == GLFW_PRESS && button == 0);
}

void OnScrollCallback(glm::vec2 scrollOffset)
{
    camPosition.z += scrollOffset.y;
}

void RegisterInputs(GLFWwindow* window){
    auto input = new InputInterface(window, onCursorPosition, OnMouseButtonCallback, OnScrollCallback);
    input->Subscribe(GLFW_KEY_A, [](){ OnMoveCamera(glm::vec3(0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_D, [](){ OnMoveCamera(glm::vec3(-0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_W, [](){ OnMoveCamera(glm::vec3(0., -0.1, 0.));});
    input->Subscribe(GLFW_KEY_S, [](){ OnMoveCamera(glm::vec3(0., 0.1, 0.));});
    input->Subscribe(GLFW_KEY_ESCAPE, [=](){throw_exit = true;});
    input->InitKeyCallback();
}

void setProjection(glm::vec2 rotation, glm::vec3 translation){
    GLuint mvp_location = glGetUniformLocation(programID, "mvp");

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), translation);

    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    glm::mat4 MVP = Projection * ViewRotateY * Model;

    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
}

void initializeProgram(){
    std::cout << "Initialize Program" << std::endl;

    vert_shader = *new cy::GLSLShader();
    vert_shader.CompileFile("../assets/shader.vert", GL_VERTEX_SHADER);

    frag_shader = *new cy::GLSLShader();
    frag_shader.CompileFile("../assets/shader.frag", GL_FRAGMENT_SHADER);

    program = *new cy::GLSLProgram();
    program.CreateProgram();
    program.AttachShader(vert_shader);
    program.AttachShader(frag_shader);
    program.Link();
    program.Bind();

    programID = program.GetID();

    glUseProgram(programID);

    OnMoveCamera(glm::vec3(0., 0., -20.));

    setProjection(camRotation, camPosition);
}

void initializeMeshWithAssimp(){
    const char* path = "../assets/suzanne.obj";
    parsedMesh = MeshParser::Process(path);

    std::cout << "Done Parsing Mesh " << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertexBuffer, normalBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, parsedMesh.Vertices.size() * sizeof(float), parsedMesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, parsedMesh.Normals.size() * sizeof(float), parsedMesh.Normals.data(), GL_STATIC_DRAW);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, parsedMesh.Indices.size() * sizeof(unsigned int), parsedMesh.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void draw(GLFWwindow* window){
    timer = glfwGetTime();

    GLint time_location = glGetUniformLocation(programID, "timer");
    glUniform1f(time_location, (float)timer);

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

    GLFWwindow* window;
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

    RegisterInputs(window);

    glEnable (GL_DEPTH_TEST);

    /* TODO Initialize Buffer Objects */
    initializeMeshWithAssimp();
    initializeProgram();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if(throw_exit){
            std::cout << "Exit by throw_exit" << std::endl;
            glfwTerminate();
            return 0;
        }

        draw(window);
    }

    glfwTerminate();
    return 0;
}