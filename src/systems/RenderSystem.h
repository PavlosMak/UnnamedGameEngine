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

        //Get the lights from the scene
        std::vector<Light> lights;
        std::vector<glm::vec3> lightPos;
        auto lightView = registry.view<LightComponent, TransformComponent>();
        for (auto lightEntity : lightView) {
            lights.push_back(lightView.get<LightComponent>(lightEntity).light);
            lightPos.push_back(registry.get<TransformComponent>(lightEntity).localTransform.pos);
        }

        clearScreen();

        // Adjust size of window
        Camera* cam = camera.getComponent<CameraComponent>().camera;
        glViewport(0, 0, windowSize.x, windowSize.y);
        cam->updateAspectRatio(aspectRatio);

        // access components with a mesh, localTransform and material
        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        // compute view projection matrix
        // TODO get world localTransform properly
        glm::mat4 vpMatrix;
        auto camTransform = camera.getComponent<TransformComponent>().localTransform;
        camera.getComponent<CameraComponent>().camera->getViewProjectionMatrix(vpMatrix, camTransform);

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

            materialComponent.material->bindMaterial(camera.getComponent<TransformComponent>().localTransform.pos, lights,lightPos);

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(worldTransform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            meshRenderer.mesh.draw();
        }

    }

};
