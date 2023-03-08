#include <iostream>
#include "ShaderManager.h"

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

const Shader &ShaderManager::getShader(SHADER_TYPE type) {
    //Assert shader exists
    return m_typeToShader[type];
}
