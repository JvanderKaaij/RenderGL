#include <fstream>
#include "MaterialInterface.h"
#include "glad/glad.h"

Shaders MaterialInterface::CompileShaders(std::string vertex_path, std::string fragment_path) {

    Shaders shaders;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::ifstream vert_file(vertex_path);
    std::string vert_str((std::istreambuf_iterator<char>(vert_file)),
                    std::istreambuf_iterator<char>());

    const char* vertexShaderSourceCstr = vert_str.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceCstr, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::ifstream frag_file(fragment_path);
    std::string frag_str((std::istreambuf_iterator<char>(frag_file)),
                         std::istreambuf_iterator<char>());
    const char* fragmetShaderSourceCstr = frag_str.c_str();
    glShaderSource(fragShader, 1, &fragmetShaderSourceCstr, NULL);
    glCompileShader(fragShader);

    shaders.vertShader = vertexShader;
    shaders.fragShader = fragShader;

    return shaders;
}