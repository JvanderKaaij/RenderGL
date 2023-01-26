//
// Created by Joey on 26/01/2023.
//

#include <fstream>
#include "MaterialInterface.h"
#include "glad/glad.h"

Shaders MaterialInterface::CompileShaders() {

    Shaders shaders;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream vert_file("../assets/shader.vert");
    std::string vert_str((std::istreambuf_iterator<char>(vert_file)),
                    std::istreambuf_iterator<char>());

    const char* vertexShaderSourceCstr = vert_str.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceCstr, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream frag_file("../assets/shader.frag");
    std::string frag_str((std::istreambuf_iterator<char>(frag_file)),
                         std::istreambuf_iterator<char>());
    const char* fragmetShaderSourceCstr = frag_str.c_str();
    glShaderSource(fragShader, 1, &fragmetShaderSourceCstr, NULL);
    glCompileShader(fragShader);

    shaders.vertShader = vertexShader;
    shaders.fragShader = fragShader;

    return shaders;
}