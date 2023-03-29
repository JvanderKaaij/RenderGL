cmake_minimum_required(VERSION 3.1)

project(imguizmo)

add_library(imguizmo
        ImGuizmo.cpp
        GraphEditor.cpp
        ImCurveEdit.cpp
        ImGradient.cpp
        ImSequencer.cpp
        )

include_directories(libraries/imgui)

target_link_libraries(imguizmo imgui)

target_include_directories(imguizmo PUBLIC .)