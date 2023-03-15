//
// Created by Joey on 15/03/2023.
//

#ifndef RENDERGL_FRAMEBUFFER_H
#define RENDERGL_FRAMEBUFFER_H


#include "glad/glad.h"
#include "Texture.h"


class FrameBuffer{
public:
    FrameBuffer(int width, int height);
    GLuint id;
    Texture* texture;
};


#endif //RENDERGL_FRAMEBUFFER_H
