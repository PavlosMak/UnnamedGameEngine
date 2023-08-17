#pragma once

#include "../materials/Texture.h"

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
    int createTexture(const std::filesystem::path& filePath);

    void bind(int textId, GLint slot);

private:
    std::vector<Texture> texturePool;
};

