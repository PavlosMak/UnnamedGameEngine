#include "Light.h"

void Light::setPosition(glm::vec3 &newPosition) {
    m_position = newPosition;
}

void Light::setColor(glm::vec3 &newColor) {
    m_color = newColor;
}

glm::vec3 Light::getPosition() {
    return m_position;
}

glm::vec3 Light::getColor() {
    return m_color;
}
