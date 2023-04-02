#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager *TextureManager::getInstance() {
    if(instance == nullptr) {
        instance = new TextureManager();
    }
    return instance;
}

int TextureManager::createTexture(std::filesystem::path filePath, GLuint clampMode) {
    texturePool.push_back(Texture(filePath));
    return texturePool.size();
}

void TextureManager::bind(int textId, GLint slot) {
    texturePool[textId-1].bind(slot);
}
