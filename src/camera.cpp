#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Transform.h"

void Camera::getProjectionMatrix(glm::mat4 &pMatrix) const {
    pMatrix = glm::perspective(glm::radians(fov), m_aspectRatio, 0.01f, 30.0f);
}

void Camera::updateAspectRatio(float aspectRatio) {
    this->m_aspectRatio = aspectRatio;
}

void Camera::getViewProjectionMatrix(glm::mat4 &vpMatrix, Transform &transform) {
    glm::mat4 pMatrix ;
    this->getProjectionMatrix(pMatrix);

    //TODO: We ideally want to support the up vector changing, for that get the rotation out of the localTransform and apply it to (0,1,0)
    vpMatrix = pMatrix * glm::lookAt(transform.pos, transform.pos + transform.forward(), glm::vec3(0, 1, 0));
}
