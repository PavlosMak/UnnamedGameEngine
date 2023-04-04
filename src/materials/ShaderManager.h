#pragma once

#include "unordered_map"
#include "framework/shader.h"


enum SHADER_TYPE {
    SOLID_COLOR = 0,
    NORMAL_AS_COLOR = 1,
    PHONG = 3,
    PBR = 4,
    TEXTURED_PBR = 5,
    TOON = 6,
    OSCILLATING_PBR = 7,
    HEIGHT_MAPPED = 8
};

class ShaderManager {
public:
    ShaderManager() = default;

    void loadShader(const std::filesystem::path &vertexShaderFile, const std::filesystem::path &fragmentShaderFile,
                    SHADER_TYPE type);

    const Shader &getShader(SHADER_TYPE type);

private:
    std::unordered_map<SHADER_TYPE, const Shader> m_typeToShader;
};


