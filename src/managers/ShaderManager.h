#pragma once

#include "unordered_map"
#include "set"
#include "framework/shader.h"


enum SHADER_TYPE {
    SOLID_COLOR = 0,
    NORMAL_AS_COLOR = 1,
    PHONG = 3,
    PBR = 4,
    TEXTURED_PBR = 5,
    TOON = 6,
    OSCILLATING_PBR = 7,
    HEIGHT_MAPPED = 8,
    SDF
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
            {"SOLID_COLOR", SOLID_COLOR},
            {"NORMAL_AS_COLOR", NORMAL_AS_COLOR},
            {"PHONG", PHONG},
            {"PBR", PBR},
            {"TEXTURED_PBR", TEXTURED_PBR},
            {"TOON", TOON},
            {"OSCILLATING_PBR", OSCILLATING_PBR},
            {"HEIGHT_MAPPED", HEIGHT_MAPPED},
            {"SDF", SDF}
    };
};


