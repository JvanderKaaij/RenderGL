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
    void InitKeyCallback();
    void Subscribe(int key, std::function<void()> callback);
    void OnMoveCamera(glm::vec3 translation);
    void OnCursorPosition(glm::vec2 position);
    void OnMouseButton(int button, int action, int mods);
    void OnScroll(glm::vec2 scrollOffset);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void OnMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    std::map<int, std::function<void()>> keyCallbacks;
    bool lMouseBtn = false;
    bool lMouseBtnCntrl = false;
    double xMousePos = 0;
    double yMousePos = 0;
    GLFWwindow* m_window;
    Scene* m_scene;
};


#endif //RENDERGL_INPUTINTERFACE_H
