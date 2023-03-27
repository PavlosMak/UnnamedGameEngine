#pragma once
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Transform {

    // in degrees
    glm::vec3 rotation;
    glm::vec3 pos;
    glm::vec3 scale;

    Transform();
    Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);

    glm::mat4 transform() const;

    glm::mat4 rotationMatrix() const;

    glm::vec3 forward() const;
};
