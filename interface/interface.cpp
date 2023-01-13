#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <cmath>
#include "cyCodeBase/cyVector.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyGL.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

static bool throw_exit = false;
static double timer = 0;
unsigned int vao;
int width = 640;
int height = 480;


cy::TriMesh mesh;
cy::GLSLShader vert_shader;
cy::GLSLShader frag_shader;
cy::GLSLProgram program;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS){
        std::cout << "Button Pressed";
    }else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        throw_exit = true;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
//    std::cout << "mouse_x: " + std::to_string(xpos) << std::endl;
//    std::cout << "mouse_y " + std::to_string(ypos) << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        std::cout << "mouse down" << std::endl;
}

void InitPerspectiveProj(GLuint id)
{
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//    GLuint mvp_location = glGetUniformLocation(id, "mvp");
//    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &projection[0][0]);

    GLuint tpos_loc = glGetUniformLocation(id, "tpos");
    glm::vec3 myVec3(.4, .0, 0.0);
    glUniform3fv(tpos_loc, 1, glm::value_ptr(myVec3));
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


    glUseProgram(program.GetID());

    InitPerspectiveProj(program.GetID());
}

void initializeMesh(){
    mesh = *new cy::TriMesh();
    mesh.LoadFromFileObj("../assets/teapot.obj");
    std::cout << mesh.NV() << " vertices loaded" << std::endl;

    cy::Vec3<float> Vertices[mesh.NF()];

    for(int i=0;i<mesh.NV();i++){
        Vertices[i] = cy::Vec3<float>(mesh.V(i)[0], mesh.V(i)[1], mesh.V(i)[2]);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}


void draw(GLFWwindow* window){
    timer = glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Render here */
    glDrawArrays(GL_POINTS, 0, mesh.NV());
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

    /* register input callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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

