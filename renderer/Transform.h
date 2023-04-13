
#ifndef RENDERGL_TRANSFORM_H
#define RENDERGL_TRANSFORM_H

#include "glm/vec3.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Transform{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    glm::vec3 Forward(){
        glm::mat4 m = this->matrix;
        return this->position - glm::vec3(m[2][0], m[2][1], m[2][2]);
    }

    glm::vec3 GetPosition(){
        return glm::vec3(this->matrix[3][0],this->matrix[3][1],this->matrix[3][2]);
    }

    void SetPosition(glm::vec3 pos){
        position = pos;
        matrix = glm::translate(matrix, position);
    }

    void AddPosition(glm::vec3 pos){
        position += pos;
        matrix = glm::translate(matrix, position);
    }

    void SetRotation(glm::vec3 rot){
        rotation = rot;
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void AddRotation(glm::vec3 rot){
        rotation += rot;
        matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

};

#endif //RENDERGL_TRANSFORM_H
