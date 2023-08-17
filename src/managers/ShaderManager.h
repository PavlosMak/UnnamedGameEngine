#pragma once

#include "unordered_map"
#include "set"
#include "framework/shader.h"


enum SHADER_TYPE {
    NORMAL_AS_COLOR = 0,
    PBR = 1,
    TEXTURED_PBR = 2,
    OSCILLATING_PBR = 3,
    SDF = 4,
};

class ShaderManager {
public:

    static ShaderManager *getInstance();

    void loadShader(const std::filesystem::path &vertexShaderFile, const std::filesystem::path &fragmentShaderFile,
                    SHADER_TYPE type);

    SHADER_TYPE getShaderType(const std::string &shaderName);

    const Shader &getShader(SHADER_TYPE type);

protected:
    ShaderManager() = default;

    static ShaderManager *instance;

private:
    std::unordered_map<SHADER_TYPE, const Shader> m_typeToShader;
    std::unordered_map<std::string, SHADER_TYPE> m_nameToType = {
            {"NORMAL_AS_COLOR", NORMAL_AS_COLOR},
            {"PBR", PBR},
            {"TEXTURED_PBR", TEXTURED_PBR},
            {"OSCILLATING_PBR", OSCILLATING_PBR},
            {"SDF", SDF}
    };
};


