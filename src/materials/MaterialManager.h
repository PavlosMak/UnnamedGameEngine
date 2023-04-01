#pragma once

#include "framework/shader.h"
#include "glm/glm.hpp"
#include "Material.h"

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
private:
    std::vector<Material> materialPool;
    int lastID{0};
};
