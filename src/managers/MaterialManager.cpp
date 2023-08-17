#include "MaterialManager.h"

#include <utility>
#include "TextureManager.h"
#include "fstream"

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
    Material mat = Material(lastID, shader, SHADER_TYPE::TEXTURED_PBR, normalTex, roughTex, metalTex, albedoTex,
                            ambientTex, heightTex);
    mat.textureSlotOccupied = 5;
    mat.lightOffset = 10;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::createSDFMaterial(const Shader &shader, std::filesystem::path pathToSDF) {
    TextureManager *textureManager = TextureManager::getInstance();
    int sdfTex = textureManager->createTexture(pathToSDF);
    Material mat = Material(lastID, shader, sdfTex, true);
    mat.textureSlotOccupied = 1;
    mat.lightOffset = 0;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}


Material *MaterialManager::createHeightMappedTexturedPBRMaterial(const Shader &shader,
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
    Material mat = Material(lastID, shader, SHADER_TYPE::HEIGHT_MAPPED, normalTex, roughTex, metalTex, albedoTex,
                            ambientTex, heightTex);
    mat.textureSlotOccupied = 6;
    mat.lightOffset = 11;
    materialPool.push_back(mat);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::createTexturedOscillatingPBRMaterial(nlohmann::json textureData) {
    assert(textureData.contains("name"));
    std::string name = textureData["name"].get<std::string>();

    nlohmann::json start_texture = textureData["start_texture"];

    TextureManager *textureManager = TextureManager::getInstance();
    int normalTex = textureManager->createTexture(std::move(start_texture["normal"].get<std::string>()));
    int roughTex = textureManager->createTexture(std::move(start_texture["roughness"].get<std::string>()));
    int metalTex = textureManager->createTexture(std::move(start_texture["metal"].get<std::string>()));
    int albedoTex = textureManager->createTexture(std::move(start_texture["albedo"].get<std::string>()));
    int ambientTex = textureManager->createTexture(std::move(start_texture["ambient"].get<std::string>()));
    int heightTex = textureManager->createTexture(std::move(start_texture["height"].get<std::string>()));

    nlohmann::json end_texture = textureData["end_texture"];

    int normalTex2 = textureManager->createTexture(std::move(end_texture["normal"].get<std::string>()));
    int roughTex2 = textureManager->createTexture(std::move(end_texture["roughness"].get<std::string>()));
    int metalTex2 = textureManager->createTexture(std::move(end_texture["metal"].get<std::string>()));
    int albedoTex2 = textureManager->createTexture(std::move(end_texture["albedo"].get<std::string>()));
    int ambientTex2 = textureManager->createTexture(std::move(end_texture["ambient"].get<std::string>()));
    int heightTex2 = textureManager->createTexture(std::move(end_texture["height"].get<std::string>()));

    const Shader &shader = ShaderManager::getInstance()->getShader(SHADER_TYPE::OSCILLATING_PBR);
    Material mat = Material(lastID, shader, normalTex, roughTex, metalTex, albedoTex, ambientTex, heightTex,
                            normalTex2, roughTex2, metalTex2, albedoTex2, ambientTex2, heightTex2);
    mat.textureSlotOccupied = 10;
    mat.lightOffset = 16;
    materialPool.push_back(mat);
    this->matNameToIndex.emplace(name, materialPool.size()-1);
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
    toonMatIndex = materialPool.size() - 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::getMaterialByName(const std::string &materialName) {
    assert(this->matNameToIndex.contains(materialName));
    int index = this->matNameToIndex[materialName];
    return &this->materialPool[index];
}

void MaterialManager::loadMaterials(const std::string &materialsPath) {
    std::ifstream file(materialsPath);
    nlohmann::json data = nlohmann::json::parse(file);
    std::vector materialsData = data.get<std::vector<nlohmann::json>>();
    for (const auto& materialData : materialsData) {
        assert(materialData.contains("name"));
        assert(materialData.contains("shader"));
        const std::string shader = materialData["shader"].get<std::string>();
        SHADER_TYPE shaderType = ShaderManager::getInstance()->getShaderType(shader);
        switch (shaderType) {
            case SOLID_COLOR:
                break;
            case NORMAL_AS_COLOR:
                break;
            case PHONG:
                break;
            case PBR:
                break;
            case TEXTURED_PBR:
                break;
            case TOON:
                break;
            case OSCILLATING_PBR:
                createTexturedOscillatingPBRMaterial(materialData);
                break;
            case HEIGHT_MAPPED:
                break;
            case SDF:
                break;
        }
    }
}
