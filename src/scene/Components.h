#pragma once

#include <utility>

#include "framework/entt_imgui/imgui_entt_entity_editor.hpp"

#include "glm/glm.hpp"
#include "../mesh.h"
#include "../materials/Material.h"
#include "../camera.h"
#include "glm/ext/matrix_transform.hpp"

struct TagComponent {
    std::string name;

    TagComponent() = default;

    TagComponent(const TagComponent &) = default;

    explicit TagComponent(std::string tag) : name(std::move(tag)) {};
};

struct TransformComponent {

    // local transform
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformComponent* parent {};

    TransformComponent() = default;

    TransformComponent(const TransformComponent &) = default;

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
    : pos(pos), rotation(rotation), scale(scale), parent(nullptr) {};

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, TransformComponent* parent)
    : pos(pos), rotation(rotation), scale(scale), parent(parent) {};

    glm::mat4 localTransform() const {

        // TODO write as single product, faster?
        auto id = glm::mat4(1);
        auto translation = glm::translate(id, pos);

        auto rotX = glm::rotate(id, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        auto rotY = glm::rotate(id, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        auto rotZ = glm::rotate(id, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        auto scaleMX = glm::scale(id, this->scale);

        // scale, then rotate, then translate
        return translation * (rotX * rotY * rotZ) * scaleMX;
    }

    glm::mat4 worldTransform() const {

        if (parent == nullptr) {
            return localTransform();
        } else {
            // TODO revise order...
            return (*parent).worldTransform() * localTransform();
        }
    }

};

struct MeshRendererComponent {
    GPUMesh mesh;

    //Todo: for now we load the mesh every time, but we might be able to do something smarter when we want the same mesh to be rendered multiple times
    explicit MeshRendererComponent(std::filesystem::path filePath) : mesh(std::move(filePath)) {};

    explicit operator GPUMesh &() { return mesh; }
};

struct CameraComponent {
    Camera &camera;
    float fov = 80.0f;

    glm::vec3 lookTarget;

    CameraComponent() = default;

    CameraComponent(const CameraComponent &) = default;

    explicit CameraComponent(Camera &camera, glm::vec3 lookTarget) : camera(camera), lookTarget(lookTarget) {};

    void getViewProjectionMatrix(glm::mat4 &vpMatrix, glm::mat4 &transform) const {
        glm::mat4 pMatrix;
        camera.getProjectionMatrix(pMatrix, fov);
        auto position = glm::vec3(transform[3]);
        //TODO: We ideally want to support the up vector changing, for that get the rotation out of the transform and apply it to (0,1,0)
        vpMatrix = pMatrix * glm::lookAt(position, lookTarget, glm::vec3(0, 1, 0));
    }
};

struct MaterialComponent {
    Material material;

    MaterialComponent() = delete;

    explicit MaterialComponent(const Material &material) : material(material) {};
};

struct WasdComponent {
    float movementSpeed = 0.1f;

    WasdComponent() : movementSpeed(0.0f) {};

    explicit WasdComponent(float movementSpeed) : movementSpeed(movementSpeed) {};
};

