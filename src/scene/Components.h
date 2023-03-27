#pragma once

#include <utility>

#include "framework/entt_imgui/imgui_entt_entity_editor.hpp"

#include "glm/glm.hpp"
#include "../mesh.h"
#include "../materials/Material.h"
#include "../camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "../Transform.h"

struct TagComponent {
    std::string name;

    TagComponent() = default;

    TagComponent(const TagComponent &) = default;

    explicit TagComponent(std::string tag) : name(std::move(tag)) {};
};

struct TransformComponent {

    Transform localTransform;
    TransformComponent* parent {};

    TransformComponent() = default;

    TransformComponent(const TransformComponent &) = default;

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
    : localTransform(Transform(pos, rotation, scale)), parent(nullptr) {};

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, TransformComponent* parent)
    : localTransform(Transform(pos, rotation, scale)), parent(parent) {};

    glm::mat4 worldTransform() const {

        if (parent == nullptr) {
            return localTransform.transform();
        } else {
            return (*parent).worldTransform() * localTransform.transform();
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
    Camera *camera;

    CameraComponent() = default;

    CameraComponent(const CameraComponent &) = default;

    explicit CameraComponent(Camera* camera) : camera(camera) {};
};

struct MaterialComponent {
    std::shared_ptr<Material> material;

    MaterialComponent() = default;

    explicit MaterialComponent(std::shared_ptr<Material> material) : material(std::move(material)) {};
};

struct WasdComponent {
    float movementSpeed = 0.1f;

    WasdComponent() : movementSpeed(0.0f) {};

    explicit WasdComponent(float movementSpeed) : movementSpeed(movementSpeed) {};
};


struct LightComponent {
    Light light;

    LightComponent() {};

    explicit LightComponent(const Light &light): light(light) {};
};
