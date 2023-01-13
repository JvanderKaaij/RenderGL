#include <string>
#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "cyCodeBase/cyVector.h"
#include "cyCodeBase/cyTriMesh.h"


static bool throw_exit = false;
static double timer = 0;
unsigned int vao;
cy::TriMesh mesh;

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
    std::cout << "mouse_x: " + std::to_string(xpos) << std::endl;
    std::cout << "mouse_y " + std::to_string(ypos) << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        std::cout << "mouse down" << std::endl;
}

void initialize(){
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

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
    initialize();

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

