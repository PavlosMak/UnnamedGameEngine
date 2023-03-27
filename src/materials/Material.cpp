#include "Material.h"
#include "glm/gtc/type_ptr.hpp"

glm::vec3 Material::getColor() const {
    return m_color;
}

const Shader &Material::getShader() const {
    return m_shader;
}

void Material::setColor(glm::vec3 &color) {
    m_color = glm::vec3(color.x, color.y, color.z);
}

void Material::bindMaterial(glm::vec3 &cameraPosition, std::vector<Light> &lights, std::vector<glm::vec3> &lightPositions) {
    m_shader.bind();
    glm::vec3 lightColor = lights[0].getColor();
    switch (m_shaderType) {
        case SOLID_COLOR:
            glUniform3fv(3, 1, glm::value_ptr(m_color));
            break;
        case PHONG:
            glUniform3fv(3, 1, glm::value_ptr(lightPositions[0]));
            glUniform3fv(4, 1, glm::value_ptr(lightColor));
            glUniform3fv(5, 1, glm::value_ptr(cameraPosition));
            glUniform3fv(6, 1, glm::value_ptr(m_color));
            glUniform1f(7, m_shininess);
            break;
        case PBR:
            glUniform3fv(3, 1, glm::value_ptr(lightPositions[0]));
            glUniform3fv(4, 1, glm::value_ptr(lightColor));
            glUniform3fv(5, 1, glm::value_ptr(cameraPosition));
            glUniform3fv(6, 1, glm::value_ptr(m_color));
            glUniform1f(7, m_roughness);
            glUniform1f(8, m_metallic);
            break;
        case NORMAL_AS_COLOR:
            break;
    }
}

void Material::setShininess(float shininess) {
    m_shininess = shininess;
}

float Material::getShininess() const {
    return m_shininess;
}



