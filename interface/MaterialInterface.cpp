
#include <vector>
#include "MaterialInterface.h"
#include "../renderer/Scene.h"


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
    unsigned char* imgData = stbi_load(full_path.c_str(), &width, &height, &nrChannels, 0);
    texture->data = imgData;
    texture->width = width;
    texture->height = height;

    glGenTextures(1, &texture->textureID);
    glBindTexture(GL_TEXTURE_2D, texture->textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this diffuseTexture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//generate bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//What to do with outside coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(texture->data);

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

void MaterialInterface::LoadShadowMapTexture() {
    glGenFramebuffers(1, &Scene::directional_light.depthMapFBO);

    glGenTextures(1, &Scene::directional_light.shadowMapID);
    glBindTexture(GL_TEXTURE_2D, Scene::directional_light.shadowMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Scene::SHADOW_WIDTH, Scene::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    //Enable Bilinear filtering for shadow map should be done only when enabling these parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, Scene::directional_light.depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Scene::directional_light.shadowMapID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MaterialInterface::LoadRenderTexture() {

}
