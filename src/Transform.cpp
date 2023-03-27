#include "Transform.h"

Transform::Transform() : pos(glm::vec3(0)), rotation(glm::vec3(0)), scale(glm::vec3(1)) {}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) : pos(pos), rotation(rotation), scale(scale) {}

glm::mat4 Transform::transform() const {

    // TODO write as single product, faster?
    auto id = glm::mat4(1);
    auto translation = glm::translate(id, pos);
    auto rot = rotationMatrix();
    auto scaleMX = glm::scale(id, this->scale);

    // scale, then rotate, then translate
    return translation * rot * scaleMX;
}

glm::mat4 Transform::rotationMatrix() const {
    auto id = glm::mat4(1);
    auto rotX = glm::rotate(id, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    auto rotY = glm::rotate(id, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    auto rotZ = glm::rotate(id, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    return rotX * rotY * rotZ;
}

glm::vec3 Transform::forward() const {
    return glm::vec3(0);
}

