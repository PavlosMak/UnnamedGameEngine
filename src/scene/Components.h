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

    Transform transform;

    TransformComponent() = default;

    TransformComponent(const TransformComponent &) = default;

    explicit TransformComponent(Transform *parent)
            : transform(Transform(glm::vec3(0), glm::vec3(0), glm::vec3(1), parent)) {};

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
            : transform(Transform(pos, rotation, scale)) {};

    explicit TransformComponent(const glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, Transform *parent)
            : transform(Transform(pos, rotation, scale, parent)) {};

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

    explicit CameraComponent(Camera *camera) : camera(camera) {};
};

struct MaterialComponent {
    Material *material;

    MaterialComponent() = default;

    explicit MaterialComponent(Material *material) : material(std::move(material)) {};
};

struct PlayerComponent {
    Material *basicMaterial;
    Material *toonMaterial;
    bool isToon = false;
    float distanceToCarmack;

    PlayerComponent(Material *basicMaterial, Material *toonMaterial) : basicMaterial(std::move(basicMaterial)),
                                                                       toonMaterial(std::move(toonMaterial)) {};
};

struct PuzzleObjectComponent {
    Material *material0;
    Material *material1;
    Material *material2;

    int key = 3;
    int solved = false;
    int currentActive = 0;

    PuzzleObjectComponent(Material *material0,
                          Material *material1,
                          Material *material2, int key) :
            material0(material0), material1(material1),
            material2(material2), key(key) {}

};

struct WasdComponent {
    float movementSpeed = 0.1f;

    WasdComponent() : movementSpeed(0.0f) {};

    explicit WasdComponent(float movementSpeed) : movementSpeed(movementSpeed) {};
};


struct LightComponent {
    Light light;

    LightComponent() {};

    explicit LightComponent(const Light &light) : light(light) {};
};
