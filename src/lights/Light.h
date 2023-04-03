#pragma once

#include "glm/vec3.hpp"

class Light {
public:
    Light() = default;


    Light(glm::vec3 color) : m_color(color) {};
    Light(glm::vec3 color, bool isCone) : m_color(color), m_isCone(isCone) {};

    void setColor(glm::vec3 newColor);

    glm::vec3 getColor();
    bool isCone();
    void setIsCone(bool isCone);

private:
    glm::vec3 m_color{1.0f};
    bool m_isCone{false};
};


