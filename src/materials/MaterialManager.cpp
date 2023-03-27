#include "MaterialManager.h"


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

std::shared_ptr<Material> MaterialManager::createPBRMaterial(const Shader& shader,glm::vec3 albedo, float roughness, float metallic) {
    Material pbrMaterial = Material(shader, albedo, roughness, metallic);
    materialPool.push_back(pbrMaterial);
    return std::shared_ptr<Material>(&materialPool[materialPool.size()-1]);
}