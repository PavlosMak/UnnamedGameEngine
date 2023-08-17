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

    void loadMaterials(const std::string &materialsPath);

    //TODO: We might want to add more methods for the other types of materials (as we need them)

    //Return a point to a material
    Material *createDebugMaterial(nlohmann::json debugData);

    Material *createPBRMaterial(nlohmann::json pbrData);

    Material *createTexturedPBRMaterial(nlohmann::json textureData);

    Material *createTexturedOscillatingPBRMaterial(nlohmann::json textureData);

    Material *createSDFMaterial(nlohmann::json sdfData);

    Material *getMaterialByName(const std::string &materialName);

private:
    std::vector<Material> materialPool;
    std::unordered_map<std::string, int> matNameToIndex;
    int lastID{0};
};
