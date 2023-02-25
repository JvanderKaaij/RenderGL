
#include "MaterialInterface.h"


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

Texture MaterialInterface::LoadTexture(aiTextureType type, aiMaterial* material){
    Texture texture;
    aiString texture_path;

    if (material->GetTexture(type, 0, &texture_path) == AI_SUCCESS)
    {
        int width, height, nrChannels;
        std::string image_path = texture_path.data;
        std::string full_path = "../assets/" + image_path;
        std::cout << "Trying to load texture: " << full_path << std::endl;
        unsigned char* imgData = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 0);
        texture.data = imgData;
        texture.width = width;
        texture.height = height;
    }
    return texture;
}

Texture MaterialInterface::LoadTexture(std::string full_path){
    Texture texture;
    std::cout << "Trying to load texture: " << full_path << std::endl;
    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 0);
    texture.data = imgData;
    texture.width = width;
    texture.height = height;
    return texture;
}