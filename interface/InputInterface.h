//
// Created by Joey on 18/01/2023.
//

#ifndef RENDERGL_INPUTINTERFACE_H
#define RENDERGL_INPUTINTERFACE_H

#include <iostream>
#include <functional>
#include <map>
#include "GLFW/glfw3.h"

class InputInterface {
public:
    InputInterface(GLFWwindow* window);
    void Subscribe(int key, std::function<void()> callback);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    std::map<int, std::function<void()>> keyCallbacks;
};


#endif //RENDERGL_INPUTINTERFACE_H
