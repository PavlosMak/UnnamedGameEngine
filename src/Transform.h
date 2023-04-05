#pragma once
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

struct Transform {

    // local transform
    glm::vec3 rotation;
    glm::vec3 pos;
    glm::vec3 scale;

    // parent
    Transform* parent;

    Transform();
    Transform(Transform* parent);
    Transform(glm::vec3 pos);
    Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale);
    Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, Transform* parent);

    glm::mat4 transform() const;
    glm::mat4 worldTransform() const;

    glm::mat4 rotationMatrix() const;

    glm::vec3 forward() const;
    glm::vec3 up() const;
    glm::vec3 right() const;
};
