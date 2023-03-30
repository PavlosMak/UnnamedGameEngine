#include "MaterialManager.h"

#include <utility>
#include "TextureManager.h"

MaterialManager* MaterialManager::instance = nullptr;

MaterialManager *MaterialManager::getInstance() {
    if(instance == nullptr) {
        instance = new MaterialManager();
    }
    return instance;
}

std::shared_ptr<Material> MaterialManager::createDebugMaterial(Shader& shader) {
    Material normalMaterial = Material(lastID, shader,
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    materialPool.push_back(normalMaterial);
    lastID += 1;
    return std::shared_ptr<Material>(&materialPool[materialPool.size()-1]);
}

Material* MaterialManager::createPBRMaterial(const Shader& shader,glm::vec3 albedo, float roughness, float metallic, float ambient) {
    Material pbrMaterial = Material(lastID, shader, albedo, roughness, metallic, ambient);
    pbrMaterial.lightOffset = 8;
    materialPool.push_back(pbrMaterial);
    lastID += 1;
    return &materialPool[materialPool.size()-1];
}

Material* MaterialManager::createTexturedPBRMaterial(const Shader &shader,
                                                                     std::filesystem::path normalMap,
                                                                     std::filesystem::path roughnessMap,
                                                                     std::filesystem::path metallicMap,
                                                                     std::filesystem::path albedoPath,
                                                                     std::filesystem::path aoPath,
                                                                     std::filesystem::path heightMap) {
    TextureManager* textureManager = TextureManager::getInstance();
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
    return &(materialPool[materialPool.size()-1]);
}