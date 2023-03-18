cmake_minimum_required(VERSION 3.1)

project(imgui)

add_library(imgui
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        )

target_include_directories(imgui PUBLIC .)