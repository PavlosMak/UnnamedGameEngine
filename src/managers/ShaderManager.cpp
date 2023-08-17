#include <iostream>
#include "ShaderManager.h"

ShaderManager *ShaderManager::instance = nullptr;

void ShaderManager::loadShader(const std::filesystem::path &vertexShaderFile,
                               const std::filesystem::path &fragmentShaderFile, SHADER_TYPE type) {
    try {
        ShaderBuilder builder;
        builder.addStage(GL_VERTEX_SHADER, vertexShaderFile);
        builder.addStage(GL_FRAGMENT_SHADER, fragmentShaderFile);
        m_typeToShader.emplace(type, builder.build());
    } catch (ShaderLoadingException &e) {
        std::cerr << e.what() << std::endl;
    }
}

ShaderManager* ShaderManager::getInstance() {
    if (instance == nullptr) {
        instance = new ShaderManager();
    }
    return instance;
}

const Shader &ShaderManager::getShader(SHADER_TYPE type) {
    assert(m_typeToShader.contains(type));
    return m_typeToShader[type];
}

SHADER_TYPE ShaderManager::getShaderType(const std::string &shaderName) {
    assert(this->m_nameToType.contains(shaderName));
    return this->m_nameToType[shaderName];
}
