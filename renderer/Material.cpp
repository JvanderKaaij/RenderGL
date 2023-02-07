#include "Mesh.h"
#include "Material.h"
#include "../interface/MaterialInterface.h"

Material::Material(Mesh mesh){
    this->shaders = MaterialInterface::CompileShaders("../assets/shader.vert", "../assets/shader.frag");
    this->diffuseTexture = MaterialInterface::LoadTexture(aiTextureType_DIFFUSE, mesh.meshMaterial);
    this->specularTexture = MaterialInterface::LoadTexture(aiTextureType_SPECULAR, mesh.meshMaterial);

    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->shaders.vertShader);
    glAttachShader(this->programID, this->shaders.fragShader);
    glLinkProgram(this->programID);
    glUseProgram(this->programID);

    //Assign diffuseTexture
    unsigned int diffuseTexture;
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &diffuseTexture);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this diffuseTexture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->diffuseTexture.width, this->diffuseTexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->diffuseTexture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//generate bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//What to do with outside coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint diffuseTextureUnitLocation = glGetUniformLocation(this->programID, "diffuseTexture");
    glUniform1i(diffuseTextureUnitLocation, 0);

    //Assign specularTexture
    unsigned int specularTexture;

    glActiveTexture(GL_TEXTURE1);

    glGenTextures(1, &specularTexture);
    glBindTexture(GL_TEXTURE_2D, specularTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this diffuseTexture object

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->specularTexture.width, this->specularTexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->specularTexture.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//generate bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//What to do with outside coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLint specularTextureUnitLocation = glGetUniformLocation(this->programID, "specularTexture");
    glUniform1i(specularTextureUnitLocation, 1);
}