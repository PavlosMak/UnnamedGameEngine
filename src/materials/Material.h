#ifndef COMPUTERGRAPHICS_MATERIAL_H
#define COMPUTERGRAPHICS_MATERIAL_H

#include "framework/shader.h"
#include "ShaderManager.h"

class Material {
public:
    Material(const Shader &shader, glm::vec3 color, SHADER_TYPE shaderType) :
            m_shader(shader), m_color(color), m_shaderType(shaderType) {}
    Material(const Shader &shader, SHADER_TYPE shaderType) : m_shader(shader), m_color(), m_shaderType(shaderType) {}

    [[nodiscard]] const Shader &getShader() const;

    [[nodiscard]] glm::vec3 getColor() const;

    void setColor(glm::vec3 &color);

    void bindMaterial();

private:
    const SHADER_TYPE m_shaderType;
    const Shader &m_shader;
    glm::vec3 m_color;
};


#endif //COMPUTERGRAPHICS_MATERIAL_H
