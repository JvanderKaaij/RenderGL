#include "InputInterface.h"
#include "imgui.h"

#include <utility>

/*
 * This class is a wrapper around the Input and exposes simple callbacks.
 * Note that I use glfwSetWindowUserPointer to set a reference to the instance of this class.
 * Now in the static OnKeyCallback, etc. I can get a reference to the instance of this class.
 *They have to be static because they are called by glfw.
*/

InputInterface::InputInterface(GLFWwindow *window, Scene* scene) {
    std::cout << "Input Initialized" << std::endl;
    m_window = window;
    m_scene = scene;
    glfwSetWindowUserPointer(window, this);
}

void InputInterface::Update(GLFWwindow *window){

    //Cursor Position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    this->OnCursorPosition(glm::vec2(xpos, ypos));

    lMouseBtn = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

}

void InputInterface::OnCursorPosition(glm::vec2 position)
{
    if(lMouseBtn){
        float deltaMouseX = (position.x - xMousePos) * 0.004f;
        float deltaMouseY = (position.y - yMousePos) * 0.004f;
        m_scene->camera.UpdateCameraDirection(deltaMouseX, deltaMouseY);
    }
    xMousePos = position.x;
    yMousePos = position.y;
}

void InputInterface::InitKeyCallback() {
    glfwSetKeyCallback(m_window, &InputInterface::OnKeyCallback);
}

void InputInterface::Subscribe(int key, std::function<void()> callback) {
    keyCallbacks[key] = std::move(callback);
}

void InputInterface::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    //warning Check if callback exists - else don't call it will crash
    if(self->keyCallbacks[key]!= nullptr){
        self->keyCallbacks[key]();
    }
}
