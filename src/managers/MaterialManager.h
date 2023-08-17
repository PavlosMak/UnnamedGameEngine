#pragma once

#include "framework/shader.h"
#include "glm/glm.hpp"
#include "../materials/Material.h"
#include "string"
#include "unordered_map"
#include "framework/json.hpp"


class MaterialManager {
protected:
    MaterialManager() : materialPool() {
        materialPool.reserve(100);
    };
    static MaterialManager *instance;

public:
    MaterialManager(MaterialManager &other) = delete;

    void operator=(const MaterialManager &) = delete;

    static MaterialManager *getInstance();

    void loadMaterials(const std::string& materialsPath);

    //TODO: We might want to add more methods for the other types of materials (as we need them)

    //Return a point to a material
    std::shared_ptr<Material> createDebugMaterial(Shader &shader);

    Material *createPBRMaterial(const Shader &shader, glm::vec4 albedo, float roughness, float metallic, float ambient);

    Material *createTexturedPBRMaterial(
            const Shader &shader,
            std::filesystem::path normalMap,
            std::filesystem::path roughnessMap,
            std::filesystem::path metallicMap,
            std::filesystem::path albedoPath,
            std::filesystem::path aoPath,
            std::filesystem::path heightMap);

    Material *createHeightMappedTexturedPBRMaterial(
            const Shader &shader,
            std::filesystem::path normalMap,
            std::filesystem::path roughnessMap,
            std::filesystem::path metallicMap,
            std::filesystem::path albedoPath,
            std::filesystem::path aoPath,
            std::filesystem::path heightMap);

    Material *createSDFMaterial(const Shader &shader, std::filesystem::path);

    Material *createTexturedOscillatingPBRMaterial(nlohmann::json textureData);


    Material *createXToonMaterial(const Shader &shader, std::filesystem::path toonTexture);

    Material *getToonMaterial();

    Material *getMaterialByName(const std::string& materialName);
private:
    std::vector<Material> materialPool;
    std::unordered_map<std::string, int> matNameToIndex;
    int lastID{0};
    int toonMatIndex{0};
};
