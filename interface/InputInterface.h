
#ifndef RENDERGL_INPUTINTERFACE_H
#define RENDERGL_INPUTINTERFACE_H

#include <iostream>
#include <functional>
#include <map>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "../renderer/Scene.h"

class InputInterface {
public:
    InputInterface(GLFWwindow* window, Scene* scene);
    void Update(GLFWwindow *window);
    void InitKeyCallback();
    void Subscribe(int key, std::function<void()> callback);
    void OnCursorPosition(glm::vec2 position);
private:
    static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    std::map<int, std::function<void()>> keyCallbacks;
    bool lMouseBtn = false;
    float xMousePos = 0;
    float yMousePos = 0;
    GLFWwindow* m_window;
    Scene* m_scene;
};


#endif //RENDERGL_INPUTINTERFACE_H
