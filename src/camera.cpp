#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void Camera::getViewProjectionMatrix(glm::mat4 &vpMatrix, float aspectRatio) {
    vpMatrix = glm::perspective(glm::radians(80.0f), aspectRatio, 0.1f, 30.0f)*
            glm::lookAt(this->m_position, this->m_lookTarget, this->m_upVector);
}

void Camera::updatePosition(const glm::vec3 &offset) {
    this->m_position += offset;
}