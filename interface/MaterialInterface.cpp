
#include <vector>
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

Texture* MaterialInterface::LoadTexture(std::string full_path){
    auto* texture = new Texture();
    std::cout << "Trying to load texture: " << full_path << std::endl;
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* imgData = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 0);

    std::cout << "Texture Width: " << width << " & Height: " << height << std::endl;

    texture->data = imgData;
    texture->width = width;
    texture->height = height;

    glGenTextures(1, &texture->textureID);
    glBindTexture(GL_TEXTURE_2D, texture->textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this diffuseTexture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//generate bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );//What to do with outside coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    stbi_image_free(texture->data);

    stbi_set_flip_vertically_on_load(false);

    return texture;
}

Texture* MaterialInterface::LoadCubeMapTexture(std::string path) {
    auto* texture = new Texture();

    glGenTextures(1, &texture->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureID);

    int width, height, nrChannels;
    unsigned char *data;

    stbi_set_flip_vertically_on_load(true);

// Load positive X
    data = stbi_load((path+"_posx.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative X
    data = stbi_load((path+"_negx.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load positive Y
    data = stbi_load((path+"_posy.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative Y
    data = stbi_load((path+"_negy.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load positive Z
    data = stbi_load((path+"_posz.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

// Load negative Z
    data = stbi_load((path+"_negz.png").c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(false);

    std::cout << "Cubemap Texture ID: " << texture->textureID << std::endl;
    return texture;
}
