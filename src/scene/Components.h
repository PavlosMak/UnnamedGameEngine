#ifndef COMPUTERGRAPHICS_COMPONENTS_H
#define COMPUTERGRAPHICS_COMPONENTS_H

#include <utility>

#include "glm/glm.hpp"
#include "../mesh.h"


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

struct MeshComponent {
    GPUMesh mesh;

    //Todo: for now we load the mesh every time, but we might be able to do
    //something smarter when we want the same mesh to be rendered multiple times
    explicit MeshComponent(std::filesystem::path filePath) : mesh(std::move(filePath)) {};

    explicit operator GPUMesh &() { return mesh; }
};


#endif //COMPUTERGRAPHICS_COMPONENTS_H
