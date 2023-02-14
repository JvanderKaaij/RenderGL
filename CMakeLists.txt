cmake_minimum_required(VERSION 3.23)
project(RenderGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package( OpenGL REQUIRED )

find_package(assimp CONFIG REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory("libraries/glfw")

add_subdirectory("libraries/glm")

include_directories("libraries/stb")

add_executable("${PROJECT_NAME}" main.cpp glad.c interface/Interface.cpp interface/Interface.h interface/InputInterface.cpp interface/InputInterface.h interface/MeshParser.cpp interface/MeshParser.h renderer/Mesh.h renderer/DirectionalLight.h interface/MaterialInterface.cpp interface/MaterialInterface.h renderer/Shaders.h renderer/Mesh.h renderer/Texture.h renderer/Material.h)

include_directories("libraries")

target_link_libraries("${PROJECT_NAME}" OpenGL::GL)

target_link_libraries("${PROJECT_NAME}" glfw ${OPENGL_LIBRARIES})

target_link_libraries("${PROJECT_NAME}" glm)

target_link_libraries("${PROJECT_NAME}" assimp::assimp)

target_link_libraries("${PROJECT_NAME}" -static)
