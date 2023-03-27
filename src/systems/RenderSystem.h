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

    glm::mat4 getWorldTransform(SRTTransformComponent t) {

        // TODO write as single product, faster?
        auto id = glm::mat4(1);
        auto translation = glm::translate(id, t.pos);
        auto rotX = glm::rotate(id, t.rotation.x, glm::vec3(1, 0, 0));
        auto rotY = glm::rotate(id, t.rotation.y, glm::vec3(0, 1, 0));
        auto rotZ = glm::rotate(id, t.rotation.z, glm::vec3(0, 0, 1));
        auto scale = glm::scale(id, t.scale);

        auto localTransform = translation * (rotX * rotY * rotZ) * scale;

        auto parent = t.parent;

        if (parent == nullptr) {
            return localTransform;
        } else {
            // TODO revise order...
            return localTransform * getWorldTransform(*parent);
        }

    }

    glm::mat4 getWorldTransform(TransformComponent t) {
        auto localTransform = t.localTransform;
        auto parent = t.parent;

        if (parent == nullptr) {
            return localTransform;
        } else {
            // TODO revise order...
            return localTransform * getWorldTransform(*parent);
        }
    }

public:


    void renderMeshesSRT(entt::registry& registry, Entity camera, glm::ivec2 windowSize, float aspectRatio) {

        clearScreen();

        Camera& cam = camera.getComponent<CameraComponent>().camera;

        // Adjust size of window
        glViewport(0, 0, windowSize.x, windowSize.y);
        cam.updateAspectRatio(aspectRatio);

        // access components with a mesh, transform and material
        auto view = registry.view<MeshRendererComponent, SRTTransformComponent, MaterialComponent>();

        // compute view projection matrix
        // TODO get world transform properly
        glm::mat4 vpMatrix;
        camera.getComponent<CameraComponent>().getViewProjectionMatrix(vpMatrix, camera.getComponent<TransformComponent>().localTransform);

        glEnable(GL_DEPTH_TEST);
        for (auto entity: view) {

            auto& transform = view.get<SRTTransformComponent>(entity);
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


    void renderMeshes(entt::registry& registry, Entity camera, glm::ivec2 windowSize, float aspectRatio) {

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
        camera.getComponent<CameraComponent>().getViewProjectionMatrix(vpMatrix, camera.getComponent<TransformComponent>().localTransform);

        glEnable(GL_DEPTH_TEST);
        for (auto entity: view) {


            TransformComponent &transform = view.get<TransformComponent>(entity);
            MeshRendererComponent &meshRenderer = view.get<MeshRendererComponent>(entity);
            MaterialComponent &materialComponent = view.get<MaterialComponent>(entity);

            const auto worldTransform = getWorldTransform(transform);

            const glm::mat4 mvpMatrix = vpMatrix * worldTransform;

            // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
            // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(transform.localTransform));

            materialComponent.material.bindMaterial();

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(transform.localTransform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            meshRenderer.mesh.draw();
            //        glUniform1i(4, GL_FALSE); This was used for the texture coordinates
        }

    }

};
