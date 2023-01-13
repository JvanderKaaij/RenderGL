#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <cmath>
#include "cyCodeBase/cyVector.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyGL.h"

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

int ToRadian(float degrees){
    return degrees * (M_PI / 180);
}

void InitPerspectiveProj()
{
    cy::Matrix4<float> mat4 = *new cy::Matrix4<float>();

    const float ar = width / height;
    const float zNear = 1.0f;
    const float zFar = 1000.0f;
    const float FOV = 30.0f;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(FOV / 2.0));

//    mat4.[0][0] = 1.0f / (tanHalfFOV * ar);
//    mat4.[0][1] = 0.0f;
//    mat4.[0][2] = 0.0f;
//    mat4.[0][3] = 0.0f;
//
//    mat4.[1][0] = 0.0f;
//    mat4.[1][1] = 1.0f / tanHalfFOV;
//    mat4.[1][2] = 0.0f;
//    mat4.[1][3] = 0.0f;
//
//    mat4.[2][0] = 0.0f;
//    mat4.[2][1] = 0.0f;
//    mat4.[2][2] = (-zNear - zFar) / zRange;
//    mat4.[2][3] = 2.0f * zFar * zNear / zRange;
//
//    mat4.[3][0] = 0.0f;
//    mat4.[3][1] = 0.0f;
//    mat4.[3][2] = 1.0f;
//    mat4.[3][3] = 0.0f;
}

void initializeProgram(){
    vert_shader = *new cy::GLSLShader();
    vert_shader.CompileFile("../assets/shader.vert", GL_VERTEX_SHADER);

    frag_shader = *new cy::GLSLShader();
    frag_shader.CompileFile("../assets/shader.frag", GL_FRAGMENT_SHADER);

    cy::Matrix4f mvp = *new cy::Matrix4f();

    program = *new cy::GLSLProgram();
    program.CreateProgram();
    program.AttachShader(vert_shader);
    program.AttachShader(frag_shader);
    program.Link();
    program.Bind();
    program.RegisterUniform(0, "mvp");
//    program.SetUniformMatrix4();
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

