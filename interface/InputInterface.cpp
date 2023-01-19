#include "InputInterface.h"

#include <utility>

/*
 * This class is a wrapper around the Input and exposes simple callbacks.
 * Note that I use glfwSetWindowUserPointer to set a reference to the instance of this class.
 * Now in the static OnKeyCallback I can get a reference to the instance of this class.
 *
*/

InputInterface::InputInterface(GLFWwindow *window, std::function<void(glm::vec2)> mousePosCallback, std::function<void(int button, int action, int mods)> mouseButtonCallback, std::function<void(glm::vec2)> mouseScrollCallback) {
    std::cout << "Input Initialized" << std::endl;
    m_window = window;
    m_mousePositionCallback = std::move(mousePosCallback); //std::move prevents a copy and just does a reference
    m_mouseButtonCallback = std::move(mouseButtonCallback);
    m_mouseScrollCallback = std::move(mouseScrollCallback);
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, InputInterface::OnMousePositionCallback);
    glfwSetMouseButtonCallback(window, InputInterface::OnMouseButtonCallback);
    glfwSetScrollCallback(window, InputInterface::OnMouseScrollCallback);
}

void InputInterface::InitKeyCallback() {
    glfwSetKeyCallback(m_window, &InputInterface::OnKeyCallback);
}

void InputInterface::OnMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->m_mousePositionCallback(glm::vec2(xpos, ypos));
}

void InputInterface::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->m_mouseButtonCallback(button, action, mods);
}

void InputInterface::OnMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->m_mouseScrollCallback(glm::vec2(xoffset, yoffset));
}

void InputInterface::Subscribe(int key, std::function<void()> callback) {
    keyCallbacks[key] = std::move(callback);
}

void InputInterface::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    //warning Check if callback exists - else don't call it will crash
    self->keyCallbacks[key]();
    std::cout << "Input Called" << std::endl;
}
