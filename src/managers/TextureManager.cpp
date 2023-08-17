#include "TextureManager.h"

TextureManager *TextureManager::instance = nullptr;

TextureManager *TextureManager::getInstance() {
    if (instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

int TextureManager::createTexture(const std::filesystem::path& filePath) {
    texturePool.emplace_back(filePath);
    return texturePool.size();
}

void TextureManager::bind(int textId, GLint slot) {
    texturePool[textId - 1].bind(slot);
}
