#ifndef RENDERGL_MAINGUI_H
#define RENDERGL_MAINGUI_H

#include "imgui.h"
#include "ImGuizmo.h"
#include "../libraries/imgui/backends/imgui_impl_glfw.h"
#include "../libraries/imgui/backends/imgui_impl_opengl3.h"
#include "../renderer/Scene.h"


class MainGUI {
public:
    bool blockMouseMove;
    void Init(GLFWwindow* window);
    void Close();
    void Draw(Scene* scene);
private:
    bool rotateTool;
    bool translateTool = true;
};


#endif //RENDERGL_MAINGUI_H
