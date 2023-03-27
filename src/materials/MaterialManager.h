#pragma once

#include "framework/shader.h"
#include "glm/glm.hpp"
#include "Material.h"

class MaterialManager {
protected:
    MaterialManager() : materialPool() {} ;
    static MaterialManager* instance;

public:
    MaterialManager(MaterialManager &other) = delete;
    void operator=(const MaterialManager &) = delete;

    static MaterialManager* getInstance();

    //TODO: We might want to add more methods for the other types of materials (as we need them)

    //Return a point to a material
    std::shared_ptr<Material> createDebugMaterial(Shader& shader);
    std::shared_ptr<Material> createPBRMaterial(const Shader& shader, glm::vec3 albedo, float roughness, float metallic);

private:
    std::vector<Material> materialPool;
};
