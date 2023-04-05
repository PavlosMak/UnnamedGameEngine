#pragma once

#include "../texture.h"

#include "vector"

class TextureManager {
protected:
    TextureManager() : texturePool() {};
    static TextureManager *instance;

public:
    TextureManager(TextureManager &other) = delete;

    void operator=(const TextureManager &) = delete;

    static TextureManager *getInstance();


    //Return a point to a material
    int createTexture(std::filesystem::path filePath, GLuint = GL_CLAMP_TO_EDGE);

    int createSkybox();

    void bind(int textId, GLint slot);

    void bindSkybox(int textId, GLint slot);

private:
    std::vector<Texture> texturePool;
};

