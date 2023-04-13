#include "MainGUI.h"

void MainGUI::Draw(Scene* scene) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuizmo::BeginFrame();

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screenSize = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
    ImGui::SetNextWindowSize(screenSize);
    ImGui::SetNextWindowPos(ImVec2(0,0));

    ImGui::Begin("Hello, teapot!", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse);

    ImGuizmo::SetOrthographic(false); // Set the projection matrix to perspective
    ImGuizmo::SetDrawlist(); // Setup draw list for rendering
    ImGuizmo::Enable(true);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),ImGui::GetWindowHeight());
    if(translateTool){
        ImGuizmo::Manipulate(glm::value_ptr(scene->camera.GetViewMatrix()), glm::value_ptr(scene->camera.GetProjectionMatrix()), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(scene->selectedTransform->matrix));
    }
    if(rotateTool){
        ImGuizmo::Manipulate(glm::value_ptr(scene->camera.GetViewMatrix()), glm::value_ptr(scene->camera.GetProjectionMatrix()), ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(scene->selectedTransform->matrix));
    }

    blockMouseMove = ImGuizmo::IsUsing();
    scene->directionalLight.Update();

    if(ImGui::Button("Translate")){
        rotateTool = false;
        translateTool = true;
    }
    if(ImGui::Button("Rotate")){
        rotateTool = true;
        translateTool = false;
    }
    ImGui::End();

    // Render the ImGui elements to the screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainGUI::Init(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    ImGui::StyleColorsDark();
}

void MainGUI::Close() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

