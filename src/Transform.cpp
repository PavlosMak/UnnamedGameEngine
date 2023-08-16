#include "Transform.h"

Transform::Transform() : pos(glm::vec3(0)), rotation(glm::vec3(0)), scale(glm::vec3(1)), parent(nullptr) {}

Transform::Transform(glm::vec3 pos) : pos(pos), rotation(glm::vec3(0)), scale(glm::vec3(1)), parent(nullptr) {}

Transform::Transform(Transform* parent) : pos(glm::vec3(0)), rotation(glm::vec3(0)), scale(glm::vec3(1)), parent(parent) {}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale) : pos(pos), rotation(rotation), scale(scale), parent(
        nullptr) {}

Transform::Transform(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, Transform* parent) : pos(pos), rotation(rotation), scale(scale), parent(parent) {}

Transform::Transform(nlohmann::json transformData) {
    this->pos = this->parseVector3(transformData["position"]);
    this->rotation = this->parseVector3(transformData["rotation"]);
    this->scale = this->parseVector3(transformData["scale"]);
}

glm::vec3 Transform::parseVector3(nlohmann::json vectorData) {
    std::vector<double> entries = vectorData.get<std::vector<double>>();
    assert(entries.size() == 3);
    return {entries[0], entries[1], entries[2]};
}

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

glm::mat4 Transform::worldTransform() const {

    if (parent == nullptr || parent == this) {
        return this->transform();
    } else {
        return (*parent).worldTransform() * this->transform();
    }
}

glm::vec3 Transform::forward() const {
    return glm::normalize(glm::vec3(this->rotationMatrix() * glm::vec4(0, 0, -1, 1)));
}

glm::vec3 Transform::up() const {
    return glm::normalize(glm::vec3(this->rotationMatrix() * glm::vec4(0, 1, 0, 1)));
}

glm::vec3 Transform::right() const {
    return glm::normalize(glm::vec3(this->rotationMatrix() * glm::vec4(1, 0, 0, 1)));
}
