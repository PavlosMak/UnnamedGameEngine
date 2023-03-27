#pragma once

#include "glm/glm.hpp"
#include "Transform.h"

class Camera {
private:
    float m_aspectRatio;
public:
    float fov = 80;

    Camera(): m_aspectRatio(1.0f) {};
    void updateAspectRatio(float aspectRatio);
    //fov in degrees
    void getProjectionMatrix(glm::mat4& pMatrix) const;

    void getViewProjectionMatrix(glm::mat4 &vpMatrix, Transform& transform);
};