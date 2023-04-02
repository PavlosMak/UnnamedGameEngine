#include "MaterialManager.h"

#include <utility>
#include "TextureManager.h"

MaterialManager *MaterialManager::instance = nullptr;

MaterialManager *MaterialManager::getInstance() {
    if (instance == nullptr) {
        instance = new MaterialManager();
    }
    return instance;
}

std::shared_ptr<Material> MaterialManager::createDebugMaterial(Shader &shader) {
    Material normalMaterial = Material(lastID, shader,
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    materialPool.push_back(normalMaterial);
    lastID += 1;
    return std::shared_ptr<Material>(&materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::createPBRMaterial(const Shader &shader, glm::vec4 albedo, float roughness, float metallic,
                                             float ambient) {
    Material pbrMaterial = Material(lastID, shader, albedo, roughness, metallic, ambient);
    pbrMaterial.lightOffset = 8;
    materialPool.push_back(pbrMaterial);
    lastID += 1;
    return &materialPool[materialPool.size() - 1];
}

Material *MaterialManager::createTexturedPBRMaterial(const Shader &shader,
                                                     std::filesystem::path normalMap,
                                                     std::filesystem::path roughnessMap,
                                                     std::filesystem::path metallicMap,
                                                     std::filesystem::path albedoPath,
                                                     std::filesystem::path aoPath,
                                                     std::filesystem::path heightMap) {
    TextureManager *textureManager = TextureManager::getInstance();
    int normalTex = textureManager->createTexture(std::move(normalMap));
    int roughTex = textureManager->createTexture(std::move(roughnessMap));
    int metalTex = textureManager->createTexture(std::move(metallicMap));
    int albedoTex = textureManager->createTexture(std::move(albedoPath));
    int ambientTex = textureManager->createTexture(std::move(aoPath));
    int heightTex = textureManager->createTexture(std::move(heightMap));
    Material mat = Material(lastID, shader, normalTex, roughTex, metalTex, albedoTex, ambientTex, heightTex);
    mat.textureSlotOccupied = 5;
    mat.lightOffset = 10;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material* MaterialManager::createTexturedOscillatingPBRMaterial(const Shader &shader, std::filesystem::path normalMap1,
                                                                std::filesystem::path roughnessMap1,
                                                                std::filesystem::path metallicMap1,
                                                                std::filesystem::path albedoPath1,
                                                                std::filesystem::path aoPath1,
                                                                std::filesystem::path heightMap1,
                                                                std::filesystem::path normalMap2,
                                                                std::filesystem::path roughnessMap2,
                                                                std::filesystem::path metallicMap2,
                                                                std::filesystem::path albedoPath2,
                                                                std::filesystem::path aoPath2,
                                                                std::filesystem::path heightMap2) {
    TextureManager *textureManager = TextureManager::getInstance();
    int normalTex = textureManager->createTexture(std::move(normalMap1));
    int roughTex = textureManager->createTexture(std::move(roughnessMap1));
    int metalTex = textureManager->createTexture(std::move(metallicMap1));
    int albedoTex = textureManager->createTexture(std::move(albedoPath1));
    int ambientTex = textureManager->createTexture(std::move(aoPath1));
    int heightTex = textureManager->createTexture(std::move(heightMap1));

    int normalTex2 = textureManager->createTexture(std::move(normalMap2));
    int roughTex2 = textureManager->createTexture(std::move(roughnessMap2));
    int metalTex2 = textureManager->createTexture(std::move(metallicMap2));
    int albedoTex2 = textureManager->createTexture(std::move(albedoPath2));
    int ambientTex2 = textureManager->createTexture(std::move(aoPath2));
    int heightTex2 = textureManager->createTexture(std::move(heightMap2));

    Material mat = Material(lastID, shader, normalTex, roughTex, metalTex, albedoTex, ambientTex, heightTex,
                            normalTex2, roughTex2, metalTex2, albedoTex2, ambientTex2, heightTex2);
    mat.textureSlotOccupied = 10;
    mat.lightOffset = 16;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::createXToonMaterial(const Shader &shader, std::filesystem::path toonTexture) {
    TextureManager *textureManager = TextureManager::getInstance();
    int toonId = textureManager->createTexture(std::move(toonTexture));
    Material mat = Material(lastID, shader, toonId);
    mat.textureSlotOccupied = 1;
    mat.lightOffset = 5;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}