cmake_minimum_required(VERSION 3.23)
project(RenderGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package( OpenGL REQUIRED )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("libraries/glfw-3.3.8")

add_executable(RenderGL main.cpp interface/interface.cpp interface/interface.h)

target_link_libraries(RenderGL OpenGL::GL)

target_link_libraries(RenderGL glfw ${OPENGL_LIBRARIES})


