#include "Material.h"
#include "glm/gtc/type_ptr.hpp"
#include "../managers/TextureManager.h"

glm::vec4 Material::getColor() const {
    return m_color;
}

const Shader &Material::getShader() const {
    return m_shader;
}

void Material::setColor(glm::vec4 &color) {
    m_color = glm::vec4(color.x, color.y, color.z, color.w);
}

void
Material::bindMaterial(glm::vec3 &cameraPosition,
                       std::vector<Light> &lights, std::vector<glm::vec3> &lightPositions) {
    m_shader.bind();
    TextureManager *texManager = TextureManager::getInstance();
    switch (TYPE) {
        case PBR:
            glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
            glUniform4fv(4, 1, glm::value_ptr(m_color));
            glUniform1f(5, m_roughness);
            glUniform1f(6, m_metallic);
            glUniform1f(7, m_ambient);
            for (int i = 0; i < lights.size(); i++) {
                glUniform3fv(lightOffset + i, 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(lightOffset + lights.size() + i, 1, glm::value_ptr(lights[i].getColor()));
                glUniform1i(lightOffset + 4 * lights.size() + i, lights[i].isCone());
            }
            break;
        case TEXTURED_PBR:
            glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
            glUniform1f(4, m_ambient);
            texManager->bind(m_normalMapId, GL_TEXTURE0);
            glUniform1i(5, 0);
            texManager->bind(m_albedoMapId, GL_TEXTURE0 + 1);
            glUniform1i(6, 1);
            texManager->bind(m_roughnessMapId, GL_TEXTURE0 + 2);
            glUniform1i(7, 2);
            texManager->bind(m_metallicMapId, GL_TEXTURE0 + 3);
            glUniform1i(8, 3);
            texManager->bind(m_ambientOcclusionMapId, GL_TEXTURE0 + 4);
            glUniform1i(9, 4);
            for (int i = 0; i < lights.size(); i++) {
                glUniform3fv(lightOffset + i, 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(lightOffset + lights.size() + i, 1, glm::value_ptr(lights[i].getColor()));
                glUniform1i(lightOffset + 4 * lights.size() + i, lights[i].isCone());
            }
            break;
        case OSCILLATING_PBR:
            glUniform3fv(3, 1, glm::value_ptr(cameraPosition));
            glUniform1f(4, m_ambient);
            texManager->bind(m_normalMapId, GL_TEXTURE0);
            glUniform1i(5, 0);
            texManager->bind(m_albedoMapId, GL_TEXTURE0 + 1);
            glUniform1i(6, 1);
            texManager->bind(m_roughnessMapId, GL_TEXTURE0 + 2);
            glUniform1i(7, 2);
            texManager->bind(m_metallicMapId, GL_TEXTURE0 + 3);
            glUniform1i(8, 3);
            texManager->bind(m_ambientOcclusionMapId, GL_TEXTURE0 + 4);
            glUniform1i(9, 4);

            texManager->bind(m_normalMapId2, GL_TEXTURE0 + 5);
            glUniform1i(10, 5);
            texManager->bind(m_albedoMapId2, GL_TEXTURE0 + 6);
            glUniform1i(11, 6);
            texManager->bind(m_roughnessMapId2, GL_TEXTURE0 + 7);
            glUniform1i(12, 7);
            texManager->bind(m_metallicMapId2, GL_TEXTURE0 + 8);
            glUniform1i(13, 8);
            texManager->bind(m_ambientOcclusionMapId2, GL_TEXTURE0 + 9);
            glUniform1i(14, 9);
            for (int i = 0; i < lights.size(); i++) {
                glUniform3fv(lightOffset + i, 1, glm::value_ptr(lightPositions[i]));
                glUniform3fv(lightOffset + lights.size() + i, 1, glm::value_ptr(lights[i].getColor()));
                glUniform1i(lightOffset + 4 * lights.size() + i, lights[i].isCone());
            }
            break;
        case SDF:
            texManager->bind(m_sdfTextureId, GL_TEXTURE0);
            glUniform1i(3, 0);
//            glUniform1f(4, 0.5);
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
