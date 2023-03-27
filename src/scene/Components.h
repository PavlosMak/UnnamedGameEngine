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
    Camera &camera;
    float fov = 80.0f;

    glm::vec3 lookTarget;

    CameraComponent() = default;

    CameraComponent(const CameraComponent &) = default;

    explicit CameraComponent(Camera &camera, glm::vec3 lookTarget) : camera(camera), lookTarget(lookTarget) {};

    void getViewProjectionMatrix(glm::mat4 &vpMatrix, Transform& transform) const {
        glm::mat4 pMatrix ;
        camera.getProjectionMatrix(pMatrix, fov);

        auto pos = transform.pos;

        auto viewDir4 = transform.rotationMatrix() * glm::vec4(0, 0, -1, 1);


        //TODO: We ideally want to support the up vector changing, for that get the rotation out of the localTransform and apply it to (0,1,0)
        vpMatrix = pMatrix * glm::lookAt(pos, pos + glm::vec3(viewDir4), glm::vec3(0, 1, 0));
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

