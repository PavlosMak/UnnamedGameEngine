#pragma once

#include "../scene/Components.h"
#include "framework/entt/entt.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../scene/Entity.h"

class RenderSystem {

private:
    inline void clearScreen() {
        // Clear the screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


public:


    void renderMeshesSRT(entt::registry& registry, Entity camera, glm::ivec2 windowSize, float aspectRatio) {

        clearScreen();

        Camera& cam = camera.getComponent<CameraComponent>().camera;

        // Adjust size of window
        glViewport(0, 0, windowSize.x, windowSize.y);
        cam.updateAspectRatio(aspectRatio);

        // access components with a mesh, transform and material
        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        // compute view projection matrix
        // TODO get world transform properly
        glm::mat4 vpMatrix;
        auto camTransform = camera.getComponent<TransformComponent>().worldTransform();
        camera.getComponent<CameraComponent>().getViewProjectionMatrix(vpMatrix, camTransform);

        glEnable(GL_DEPTH_TEST);
        for (auto entity: view) {

            auto& transform = view.get<TransformComponent>(entity);
            auto& meshRenderer = view.get<MeshRendererComponent>(entity);
            auto& materialComponent = view.get<MaterialComponent>(entity);

            auto worldTransform = transform.worldTransform();

            const glm::mat4 mvpMatrix = vpMatrix * worldTransform;

            // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
            // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(worldTransform));

            materialComponent.material.bindMaterial();

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(worldTransform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            meshRenderer.mesh.draw();
        }

    }

};
