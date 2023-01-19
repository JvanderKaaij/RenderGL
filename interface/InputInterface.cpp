#include "InputInterface.h"

/* Note that I use glfwSetWindowUserPointer to set a reference to the instance of this class.
 * Now in the static OnKeyCallback I can get a reference to the instance of this class.
*/

InputInterface::InputInterface(GLFWwindow *window) {
    std::cout << "Input Initialized" << std::endl;
    m_window = window;
    glfwSetWindowUserPointer(window, this);
}

void InputInterface::InitKeyCallback() {
    glfwSetKeyCallback(m_window, &InputInterface::OnKeyCallback);
}

void InputInterface::InitMousePositionCallback(std::function<void(glm::vec2)> callback) {
//    glfwSetCursorPosCallback(m_window, callback);
}

void InputInterface::Subscribe(int key, std::function<void()> callback) {
    keyCallbacks[key] = callback;
}

void InputInterface::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    InputInterface* self = (InputInterface*)glfwGetWindowUserPointer(window);
    //warning Check if callback exists - else don't call it will crash
    self->keyCallbacks[key]();
    std::cout << "Input Called" << std::endl;
}
