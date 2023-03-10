#ifndef COMPUTERGRAPHICS_MATERIAL_H
#define COMPUTERGRAPHICS_MATERIAL_H

#include "framework/shader.h"
#include "ShaderManager.h"
#include "../scene/Entity.h"
#include "../lights/Light.h"

class Material {
public:
    Material(const Shader &shader, glm::vec3 color, SHADER_TYPE shaderType) :
            m_shader(shader), m_color(color), m_shaderType(shaderType) {}

    Material(const Shader &shader, SHADER_TYPE shaderType) : m_shader(shader), m_color(), m_shaderType(shaderType) {}

    Material(const Shader &shader, glm::vec3 color, float shininess, SHADER_TYPE shaderType) : m_shader(shader),
                                                                                               m_color(color),
                                                                                               m_shininess(shininess),
                                                                                               m_shaderType(
                                                                                                       shaderType) {}

    [[nodiscard]] const Shader &getShader() const;

    [[nodiscard]] glm::vec3 getColor() const;

    void setColor(glm::vec3 &color);

    [[nodiscard]] float getShininess() const;
    void setShininess(float shininess);

    void bindMaterial(glm::vec3 &cameraPosition, std::vector<Light> &lights);

private:
    const SHADER_TYPE m_shaderType;
    const Shader &m_shader;
    glm::vec3 m_color{1.0f}; //m_color acts as the diffuse
    float m_shininess{0.0f};
};


#endif //COMPUTERGRAPHICS_MATERIAL_H
