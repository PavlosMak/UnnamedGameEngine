#pragma once

#include "framework/shader.h"
#include "glm/glm.hpp"
#include "Material.h"
#include "string"
#include "unordered_map"

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

    Material *createTexturedOscillatingPBRMaterial(
            const std::string& name,
            const Shader &shader,
            std::filesystem::path normalMap1,
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
            std::filesystem::path heightMap2
    );


    Material *createXToonMaterial(const Shader &shader, std::filesystem::path toonTexture);

    Material *getToonMaterial();

    Material *getMaterialByName(const std::string& materialName);
private:
    std::vector<Material> materialPool;
    std::unordered_map<std::string, int> matNameToIndex;
    int lastID{0};
    int toonMatIndex{0};
};
