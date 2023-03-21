#include "InputInterface.h"

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
    glfwSetCursorPosCallback(window, InputInterface::OnMousePositionCallback);
    glfwSetMouseButtonCallback(window, InputInterface::OnMouseButtonCallback);
    glfwSetScrollCallback(window, InputInterface::OnMouseScrollCallback);
}

void InputInterface::OnMoveCamera(glm::vec3 translation){
    m_scene->camera.transform.position += translation;
}

void InputInterface::OnCursorPosition(glm::vec2 position)
{
    if(lMouseBtn){
        m_scene->camera.transform.rotation.x += (position.x - xMousePos) * 0.01f;
        m_scene->camera.transform.rotation.y += (position.y - yMousePos) * 0.01f;
    }
    if(lMouseBtnCntrl){
        m_scene->directional_light.direction.x += (position.x - xMousePos) * 0.1f;
        m_scene->directional_light.direction.y -= (position.y - yMousePos) * 0.1f;
    }
    xMousePos = position.x;
    yMousePos = position.y;
}

void InputInterface::OnMouseButton(int button, int action, int mods)
{
    lMouseBtn = (action == GLFW_PRESS && button == 0 && mods == 0);
    lMouseBtnCntrl = (action == GLFW_PRESS && button == 0 && (mods & GLFW_MOD_CONTROL) != 0);
}

void InputInterface::InitKeyCallback() {
    glfwSetKeyCallback(m_window, &InputInterface::OnKeyCallback);
}

void InputInterface::OnMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->OnCursorPosition(glm::vec2(xpos, ypos));
}

void InputInterface::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->OnMouseButton(button, action, mods);
}

void InputInterface::OnScroll(glm::vec2 scrollOffset)
{
    m_scene->camera.transform.position.z += scrollOffset.y;
}

void InputInterface::OnMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* self = (InputInterface*)glfwGetWindowUserPointer(window);
    self->OnScroll(glm::vec2(xoffset, yoffset));
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
