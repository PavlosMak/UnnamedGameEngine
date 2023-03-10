#ifndef COMPUTERGRAPHICS_LIGHT_H
#define COMPUTERGRAPHICS_LIGHT_H


#include "glm/vec3.hpp"

class Light {
public:
    Light() = default;;

    Light(glm::vec3 position, glm::vec3 color) : m_position(position), m_color(color) {};

    void setPosition(glm::vec3 &newPosition);

    void setColor(glm::vec3 &newColor);

    glm::vec3 getPosition();

    glm::vec3 getColor();

private:
    glm::vec3 m_position{0.f};
    glm::vec3 m_color{1.0f};
};


#endif //COMPUTERGRAPHICS_LIGHT_H
