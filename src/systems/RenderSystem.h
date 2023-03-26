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


    void renderMeshes(entt::registry& registry, Entity camera, glm::ivec2 windowSize, float aspectRatio) {

        clearScreen();

        auto cam = camera.getComponent<CameraComponent>().camera;

        // Adjust size of window
        glViewport(0, 0, windowSize.x, windowSize.y);

        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        glm::mat4 vpMatrix;
        camera.getComponent<CameraComponent>().getViewProjectionMatrix(vpMatrix, camera.getComponent<TransformComponent>().transform);

        glEnable(GL_DEPTH_TEST);
        for (auto entity: view) {
            TransformComponent &transform = view.get<TransformComponent>(entity);
            MeshRendererComponent &meshRenderer = view.get<MeshRendererComponent>(entity);
            MaterialComponent &materialComponent = view.get<MaterialComponent>(entity);

            const glm::mat4 mvpMatrix = vpMatrix * transform.transform;
            // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
            // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(transform.transform));

            materialComponent.material.bindMaterial();

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(transform.transform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            meshRenderer.mesh.draw();
            //        glUniform1i(4, GL_FALSE); This was used for the texture coordinates
        }

//        // Clear the screen
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
};
