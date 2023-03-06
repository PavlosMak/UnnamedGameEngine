#include "glm/glm.hpp"

#ifndef COMPUTERGRAPHICS_CAMERA_H
#define COMPUTERGRAPHICS_CAMERA_H


class Camera {
private:
    float m_aspectRatio;
public:
    Camera(): m_aspectRatio(1.0f) {};
    void updateAspectRatio(float aspectRatio);
    //fov in degrees
    void getProjectionMatrix(glm::mat4& pMatrix, float fov) const;
};


#endif //COMPUTERGRAPHICS_CAMERA_H
