
#include <string>
#include <vector>
#include "Material.h"
#include "../../interface/MaterialInterface.h"

Material::Material(std::string vertex_path, std::string fragment_path) {
    std::cout << "Shader Vertex Path" << vertex_path << std::endl;
    std::cout << "Shader Fragment Path" << fragment_path << std::endl;

    this->shaders = MaterialInterface::CompileShaders(vertex_path.c_str(), fragment_path.c_str());
    this->programID = glCreateProgram();
    std::cout << "Material created from base with program ID" << this->programID << std::endl;
    glAttachShader(this->programID, this->shaders.vertShader);
    glAttachShader(this->programID, this->shaders.fragShader);
    glLinkProgram(this->programID);

    GLint isLinked = 0;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLchar* infoLog = new GLchar[maxLength + 1];
        glGetProgramInfoLog(this->programID, maxLength, &maxLength, &infoLog[0]);

        std::cout << std::endl << "SHADER ERROR: " << std::endl;
        std::cout << infoLog << std::endl;

        // The program is useless now. So delete it.
        glDeleteProgram(this->programID);

        // Provide the infolog in whatever manner you deem best.
        // Exit with failure.
        return;
    }

    glDeleteShader(this->shaders.vertShader);
    glDeleteShader(this->shaders.fragShader);

    glUseProgram(this->programID);
}

void Material::Draw(Transform transform, SceneUniformBlock* sceneUniforms) {
    glUseProgram(this->programID);

    glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
    ModelMatrix = glm::scale(ModelMatrix, transform.scale);
    ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    GLuint m_location = glGetUniformLocation(this->programID, "model");
    glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
}
