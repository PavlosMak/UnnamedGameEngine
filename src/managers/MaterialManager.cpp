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

Material *MaterialManager::createDebugMaterial(nlohmann::json debugData) {
    const Shader &shader = ShaderManager::getInstance()->getShader(SHADER_TYPE::NORMAL_AS_COLOR);
    std::string name = debugData["name"].get<std::string>();
    Material normalMaterial = Material(lastID, shader,
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    materialPool.push_back(normalMaterial);
    this->matNameToIndex.emplace(name, materialPool.size() - 1);
    lastID += 1;
    return &materialPool[materialPool.size() - 1];
}

Material *MaterialManager::createPBRMaterial(nlohmann::json pbrData) {
    assert(pbrData.contains("name"));
    std::string name = pbrData["name"].get<std::string>();
    const Shader &shader = ShaderManager::getInstance()->getShader(SHADER_TYPE::PBR);

    auto albedo = glm::vec4(1);
    if (pbrData.contains("albedo")) {
        std::vector<float> albedoData = pbrData["albedo"].get<std::vector<float>>();
        assert(albedoData.size() == 4);
        albedo.x = albedoData[0];
        albedo.y = albedoData[1];
        albedo.z = albedoData[2];
        albedo.w = albedoData[3];
    }

    float roughness = 0.5;
    if (pbrData.contains("roughness")) {
        roughness = pbrData["roughness"].get<float>();
    }

    float metal = 0.5;
    if (pbrData.contains("metal")) {
        metal = pbrData["metal"].get<float>();
    }

    float ambient = 0.2;
    if (pbrData.contains("ambient")) {
        ambient = pbrData["ambient"].get<float>();
    }

    Material pbrMaterial = Material(lastID, shader, albedo, roughness, metal, ambient);
    pbrMaterial.lightOffset = 8;
    materialPool.push_back(pbrMaterial);
    this->matNameToIndex.emplace(name, materialPool.size() - 1);
    lastID += 1;
    return &materialPool[materialPool.size() - 1];
}

Material *MaterialManager::createTexturedPBRMaterial(nlohmann::json textureData) {
    assert(textureData.contains("name"));
    std::string name = textureData["name"].get<std::string>();

    nlohmann::json texture = textureData["texture"];

    TextureManager *textureManager = TextureManager::getInstance();
    int normalTex = textureManager->createTexture(texture["normal"].get<std::string>());
    int roughTex = textureManager->createTexture(texture["roughness"].get<std::string>());
    int metalTex = textureManager->createTexture(texture["metal"].get<std::string>());
    int albedoTex = textureManager->createTexture(texture["albedo"].get<std::string>());
    int ambientTex = textureManager->createTexture(texture["ambient"].get<std::string>());
    int heightTex = textureManager->createTexture(texture["height"].get<std::string>());

    const Shader &shader = ShaderManager::getInstance()->getShader(SHADER_TYPE::TEXTURED_PBR);
    Material mat = Material(lastID, shader, SHADER_TYPE::TEXTURED_PBR, normalTex, roughTex, metalTex, albedoTex,
                            ambientTex, heightTex);
    mat.textureSlotOccupied = 5;
    mat.lightOffset = 10;
    materialPool.push_back(mat);
    this->matNameToIndex.emplace(name, materialPool.size() - 1);
    lastID += 1;
    return &(materialPool[materialPool.size() - 1]);
}

Material *MaterialManager::createSDFMaterial(nlohmann::json sdfData) {
    assert(sdfData.contains("name"));
    assert(sdfData.contains("texture"));

    std::string name = sdfData["name"].get<std::string>();
    std::string pathToSDF = sdfData["texture"].get<std::string>();

    const Shader& shader = ShaderManager::getInstance()->getShader(SHADER_TYPE::SDF);
    TextureManager *textureManager = TextureManager::getInstance();
    int sdfTex = textureManager->createTexture(pathToSDF);
    Material mat = Material(lastID, shader, sdfTex, true);
    mat.textureSlotOccupied = 1;
    mat.lightOffset = 0;
    materialPool.push_back(mat);
    this->matNameToIndex.emplace(name, materialPool.size() - 1);
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
    this->matNameToIndex.emplace(name, materialPool.size() - 1);
    lastID += 1;
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
    for (const auto &materialData: materialsData) {
        assert(materialData.contains("name"));
        assert(materialData.contains("shader"));
        const std::string shader = materialData["shader"].get<std::string>();
        SHADER_TYPE shaderType = ShaderManager::getInstance()->getShaderType(shader);
        switch (shaderType) {
            case NORMAL_AS_COLOR:
                createDebugMaterial(materialData);
                break;
            case PBR:
                createPBRMaterial(materialData);
                break;
            case TEXTURED_PBR:
                createTexturedPBRMaterial(materialData);
                break;
            case OSCILLATING_PBR:
                createTexturedOscillatingPBRMaterial(materialData);
                break;
            case SDF:
                createSDFMaterial(materialData);
                break;
        }
    }
}
