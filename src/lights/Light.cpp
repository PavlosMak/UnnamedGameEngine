#include "Light.h"

void Light::setColor(glm::vec3 newColor) {
    m_color = newColor;
}

glm::vec3 Light::getColor() {
    return m_color;
}

bool Light::isCone() {
    return m_isCone;
}

void Light::setIsCone(bool isCone) {
    this->m_isCone = isCone;
}
