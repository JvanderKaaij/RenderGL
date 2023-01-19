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
    InputInterface(GLFWwindow* window);
    void InitKeyCallback();
    void InitMousePositionCallback(std::function<void(glm::vec2)> callback);
    void Subscribe(int key, std::function<void()> callback);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    std::map<int, std::function<void()>> keyCallbacks;
    GLFWwindow* m_window;
};


#endif //RENDERGL_INPUTINTERFACE_H
