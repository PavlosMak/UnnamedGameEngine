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

void Material::bindMaterial() {
    m_shader.bind();
    switch (m_shaderType) {
        case SOLID_COLOR:
            glUniform3fv(3, 1, glm::value_ptr(m_color));
            break;
        case PHONG:
            glUniform3fv(5, 1, glm::value_ptr(m_color));
            break;
        case NORMAL_AS_COLOR:
            break;
    }
}

