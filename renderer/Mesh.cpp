#include <iostream>
#include "Mesh.h"
#include "../interface/MeshParser.h"


void Mesh::Parse(const char* path){
    MeshParser::Process(path, this);
}

void Mesh::InitBuffers(){
    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);

    glGenBuffers(1, &this->vertexBuffer);
    glGenBuffers(1, &this->normalBuffer);
    glGenBuffers(1, &this->textureCoordBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    std::cout << "Index of Vertex Buffer" << this->vertexBuffer << std::endl;
    std::cout << "Size of Array Buffer: " << this->Vertices.size() * sizeof(float) << std::endl;

    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(float), this->Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->Normals.size() * sizeof(float), this->Normals.data(), GL_STATIC_DRAW);

    //Textures
    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, this->TextureCoords.size() * sizeof(float), this->TextureCoords.data(), GL_STATIC_DRAW);

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(unsigned int), this->Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
}