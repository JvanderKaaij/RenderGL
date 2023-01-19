#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "cyCodeBase/cyVector.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include "InputInterface.h"

static bool throw_exit = false;
static double timer = 0;
unsigned int vao;
int width = 640;
int height = 480;

GLuint programID;

float xPos = 0.;
float zPos = 0.;

glm::vec3 camPosition = glm::vec3(0,0,0);

glm::vec2 camRotation = glm::vec2(0,0);

bool lMouseBtn = false;
double xMousePos = 0;
double yMousePos = 0;

cy::TriMesh mesh;
cy::GLSLShader vert_shader;
cy::GLSLShader frag_shader;
cy::GLSLProgram program;

std::vector<int> Indices;

void moveCamera(glm::vec3 translation){
    camPosition += translation;
    std::cout << "Callback Called";
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(lMouseBtn){
        camRotation.x += (xpos - xMousePos) * 0.01f;
        camRotation.y += (ypos - yMousePos) * 0.01f;
    }
    xMousePos = xpos;
    yMousePos = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    lMouseBtn = (action == GLFW_PRESS && button == 0);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << yoffset << std::endl;
    camPosition.z += yoffset;
}

void setProjection(glm::vec2 rotation, glm::vec3 translation)
{
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

    camPosition.z = -20.;

    setProjection(camRotation, camPosition);
}


void RegisterInputs(GLFWwindow* window){
    InputInterface* input = new InputInterface(window);
    input->Subscribe(GLFW_KEY_A, [](){moveCamera(glm::vec3(0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_D, [](){moveCamera(glm::vec3(-0.1, 0., 0.));});
    input->Subscribe(GLFW_KEY_W, [](){moveCamera(glm::vec3(0., -0.1, 0.));});
    input->Subscribe(GLFW_KEY_S, [](){moveCamera(glm::vec3(0., 0.1, 0.));});
    input->Subscribe(GLFW_KEY_ESCAPE, [=](){throw_exit = true;});
    input->InitKeyCallback();
//    input->InitMousePositionCallback(mouse_position_callback);

    glfwSetCursorPosCallback(window, mouse_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

}

void initializeMesh(){
    mesh = *new cy::TriMesh();
    mesh.LoadFromFileObj("../assets/teapot.obj");
    std::cout << mesh.NV() << " vertices loaded" << std::endl;

    // Vertices

    cy::Vec3<float> Vertices[mesh.NV()];

    for(int i=0;i<mesh.NV();i++){
        Vertices[i] = cy::Vec3<float>(mesh.V(i)[0], mesh.V(i)[1], mesh.V(i)[2]);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertexBuffer, normalBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &normalBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    //Surface Normals

    std::vector<glm::vec3> Normals;
    for(int i=0;i<mesh.NVN();i++){
        Normals.push_back(glm::vec3(mesh.VN(i)[0], mesh.VN(i)[1], mesh.VN(i)[2]));
    }

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), Normals.data(), GL_STATIC_DRAW);

    // Indices

    for(int i=0;i<mesh.NF();i++){
        Indices.push_back(mesh.F(i).v[0]);
        Indices.push_back(mesh.F(i).v[1]);
        Indices.push_back(mesh.F(i).v[2]);
    }

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), Indices.data(), GL_STATIC_DRAW);

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

    GLint x_location = glGetUniformLocation(programID, "xPos");
    glUniform1f(x_location, xPos);

    setProjection(camRotation, camPosition);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render here */

    glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, 2);
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
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

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
    initializeMesh();
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

