#ifndef COMPUTERGRAPHICS_LIGHT_H
#define COMPUTERGRAPHICS_LIGHT_H


#include "glm/vec3.hpp"

class Light {
public:
    Light() = default;

    Light(glm::vec3 color) : m_color(color) {};

    void setColor(glm::vec3 newColor);

    glm::vec3 getColor();
private:
    glm::vec3 m_color{1.0f};
};


#endif //COMPUTERGRAPHICS_LIGHT_H
