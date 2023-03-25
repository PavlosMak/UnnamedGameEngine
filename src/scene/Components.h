#ifndef COMPUTERGRAPHICS_COMPONENTS_H
#define COMPUTERGRAPHICS_COMPONENTS_H

#include <utility>

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
    glm::mat4 transform;

    //Define various constructors
    TransformComponent() = default;

    TransformComponent(const TransformComponent &) = default;

    explicit TransformComponent(const glm::mat4 transform) : transform(transform) {};

    //define ability to cast to mat4, so we can do math with the component
    explicit operator glm::mat4 &() { return transform; }
};

struct MeshRendererComponent {
    GPUMesh mesh;

    //Todo: for now we load the mesh every time, but we might be able to do something smarter when we want the same mesh to be rendered multiple times
    explicit MeshRendererComponent(std::filesystem::path filePath) : mesh(std::move(filePath)) {};

    explicit operator GPUMesh &() { return mesh; }
};

struct CameraComponent {
    Camera& camera;
    float fov = 80.0f;

    glm::vec3 lookTarget;

    CameraComponent() = delete;

    CameraComponent(const CameraComponent &) = default;

    explicit CameraComponent(Camera& camera, glm::vec3 lookTarget) : camera(camera), lookTarget(lookTarget) {};

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
    explicit MaterialComponent(const Material &material): material(material) {};
};

struct WasdComponent {
    float movementSpeed = 0.1f;

    explicit WasdComponent(float movementSpeed) : movementSpeed(movementSpeed) {};
};

#endif //COMPUTERGRAPHICS_COMPONENTS_H
