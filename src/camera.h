#include "glm/glm.hpp"

#ifndef COMPUTERGRAPHICS_CAMERA_H
#define COMPUTERGRAPHICS_CAMERA_H


class Camera {
private:
    glm::vec3 m_position;
    glm::vec3 m_lookTarget;
    glm::vec3 m_upVector;
public:
    Camera(glm::vec3 position, glm::vec3 lookTarget, glm::vec3 up): m_position(position), m_lookTarget(lookTarget), m_upVector(up) {};
    void updatePosition(const glm::vec3& offset);
    void getViewProjectionMatrix(glm::mat4& vpMatrix, float aspectRatio);
};


#endif //COMPUTERGRAPHICS_CAMERA_H
