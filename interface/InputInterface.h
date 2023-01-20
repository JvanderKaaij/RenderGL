//
// Created by Joey on 18/01/2023.
//

#ifndef RENDERGL_INPUTINTERFACE_H
#define RENDERGL_INPUTINTERFACE_H

#include <iostream>
#include <functional>
#include <map>
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"

class InputInterface {
public:
    InputInterface(GLFWwindow* window, std::function<void(glm::vec2)> mousePosCallback, std::function<void(int button, int action, int mods)> mouseButtonCallback, std::function<void(glm::vec2)> mouseScrollCallback);
    void InitKeyCallback();
    void Subscribe(int key, std::function<void()> callback);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void OnMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    std::map<int, std::function<void()>> keyCallbacks;
    std::function<void(glm::vec2)> m_mousePositionCallback;
    std::function<void(int button, int action, int mods)> m_mouseButtonCallback;
    std::function<void(glm::vec2)> m_mouseScrollCallback;
    GLFWwindow* m_window;
};


#endif //RENDERGL_INPUTINTERFACE_H
