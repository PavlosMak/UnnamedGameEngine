#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void Camera::getProjectionMatrix(glm::mat4 &pMatrix, float fov) const {
    pMatrix = glm::perspective(glm::radians(fov), m_aspectRatio, 0.1f, 30.0f);
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->m_aspectRatio = aspectRatio;
}