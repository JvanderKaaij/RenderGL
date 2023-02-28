
#ifndef RENDERGL_TEXTURE_H
#define RENDERGL_TEXTURE_H

struct Texture {
    unsigned char* data;
    int width, height, channels;
    GLenum location;
    GLuint textureID;
};

#endif