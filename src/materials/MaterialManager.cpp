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
    Material normalMaterial = Material(shader,
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    materialPool.push_back(normalMaterial);
    return std::shared_ptr<Material>(&materialPool[materialPool.size()-1]);
}

std::shared_ptr<Material> MaterialManager::createPBRMaterial(const Shader& shader,glm::vec3 albedo, float roughness, float metallic, float ambient) {
    Material pbrMaterial = Material(shader, albedo, roughness, metallic, ambient);
    materialPool.push_back(pbrMaterial);
    return std::shared_ptr<Material>(&materialPool[materialPool.size()-1]);
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
    Material mat = Material(shader, normalTex, roughTex, metalTex, albedoTex, ambientTex, heightTex);
    materialPool.push_back(mat);
    return &(materialPool[materialPool.size()-1]);
}