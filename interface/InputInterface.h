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
#include "../renderer/Scene.h"

class InputInterface {
public:
    InputInterface(GLFWwindow* window, Scene* scene);
    void Update(GLFWwindow *window);
    void InitKeyCallback();
    void Subscribe(int key, std::function<void()> callback);
    void OnMoveCamera(glm::vec3 translation);
    void OnCursorPosition(glm::vec2 position);
    void OnScroll(glm::vec2 scrollOffset);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    std::map<int, std::function<void()>> keyCallbacks;
    bool lMouseBtn = false;
    bool lKeyModCntrl = false;
    double xMousePos = 0;
    double yMousePos = 0;
    GLFWwindow* m_window;
    Scene* m_scene;
};


#endif //RENDERGL_INPUTINTERFACE_H
