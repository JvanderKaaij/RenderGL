//
// Created by Joey on 16/03/2023.
//

#ifndef RENDERGL_DEPTHFRAMEBUFFER_H
#define RENDERGL_DEPTHFRAMEBUFFER_H


#include "glad/glad.h"
#include "../Texture.h"
#include "../Scene.h"

class DepthFrameBuffer {
public:
    DepthFrameBuffer(int width, int height);
    GLuint id;
    Texture* texture;
};


#endif //RENDERGL_DEPTHFRAMEBUFFER_H
