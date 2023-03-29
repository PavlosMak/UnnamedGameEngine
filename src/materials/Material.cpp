#include "Material.h"
#include "glm/gtc/type_ptr.hpp"
#include "TextureManager.h"

glm::vec3 Material::getColor() const {
    return m_color;
}

const Shader &Material::getShader() const {
    return m_shader;
}

void Material::setColor(glm::vec3 &color) {
    m_color = glm::vec3(color.x, color.y, color.z);
}

int Material::bindMaterial(glm::vec3 &cameraPosition, std::vector<Light> &lights, std::vector<glm::vec3> &lightPositions) {
    m_shader.bind();

    TextureManager* texManager = TextureManager::getInstance();
    int textureSlotOccupied = 0;
    switch (m_shaderType) {
        case SOLID_COLOR:
            glUniform3fv(3, 1, glm::value_ptr(m_color));
            break;
        case PHONG:
            glUniform3fv(3, 1, glm::value_ptr(lightPositions[0]));
            glUniform3fv(4, 1, glm::value_ptr(lights[0].getColor()));
            glUniform3fv(5, 1, glm::value_ptr(cameraPosition));
            glUniform3fv(6, 1, glm::value_ptr(m_color));
            glUniform1f(7, m_shininess);
            break;
        case PBR:
            glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
            glUniform3fv(4, 1, glm::value_ptr(m_color));
            glUniform1f(5, m_roughness);
            glUniform1f(6, m_metallic);
            glUniform1f(7, m_ambient);
            for(int i = 0; i < lights.size(); i++) {
                glUniform3fv(8 + i, 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(8 + lights.size() + i, 1, glm::value_ptr(lights[i].getColor()));
            }
            break;
        case TEXTURED_PBR:
            glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
            glUniform1f(4, m_ambient);
            texManager->bind(m_normalMapId,GL_TEXTURE0);
            glUniform1i(5,0);
            texManager->bind(m_albedoMapId,GL_TEXTURE0+1);
            glUniform1i(6,1);
            texManager->bind(m_roughnessMapId,GL_TEXTURE0+2);
            glUniform1i(7,2);
            texManager->bind(m_metallicMapId,GL_TEXTURE0+3);
            glUniform1i(8,3);
            texManager->bind(m_ambientOcclusionMapId,GL_TEXTURE0+4);
            glUniform1i(9,4);
            for(int i = 0; i < lights.size(); i++) {
                glUniform3fv(10 + i, 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(10 + lights.size() + i, 1, glm::value_ptr(lights[i].getColor()));
            }
            textureSlotOccupied = 5;
            break;
        case NORMAL_AS_COLOR:
            break;
    }
    return textureSlotOccupied;
}

void Material::setShininess(float shininess) {
    m_shininess = shininess;
}

float Material::getShininess() const {
    return m_shininess;
}

float Material::getRoughness() const {
    return m_roughness;
}

void Material::setRoughness(float roughness) {
    m_roughness = roughness;
}

float Material::getMetallic() const {
    return m_metallic;
}

void Material::setMetallic(float metallic) {
    m_metallic = metallic;
}

float Material::getAmbient() const {
    return m_ambient;
}

void Material::setAmbient(float ambient) {
    m_ambient = ambient;
}
