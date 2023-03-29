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

    std::vector<GLuint> shadowMaps;
    std::vector<glm::mat4> lightVPs;
//    glm::mat4 lightVp;

//    GLuint shadowMap;

public:


    void createShadowMaps(Shader &shadowShader, entt::registry &registry) {
        //Get the lights from the scene
        std::vector<Light> lights;
        std::vector<glm::vec3> lightPos;

        //Get all lights
        auto lightView = registry.view<LightComponent, TransformComponent>();

        // access components with a mesh, localTransform and material
        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        //We are going to be moving this camera around to mimic light directionality
        Camera lightCamera = Camera();
        for (auto lightEntity: lightView) {
            Transform &transform = registry.get<TransformComponent>(lightEntity).localTransform;
            lights.push_back(lightView.get<LightComponent>(lightEntity).light);
            lightPos.push_back(transform.pos);

            // === Create Shadow Texture ===
            GLuint texShadow;
            const int SHADOWTEX_WIDTH = 1024;
            const int SHADOWTEX_HEIGHT = 1024;
            glCreateTextures(GL_TEXTURE_2D, 1, &texShadow);
            glTextureStorage2D(texShadow, 1, GL_DEPTH_COMPONENT32F, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

            // Set behaviour for when texture coordinates are outside the [0, 1] range.
            glTextureParameteri(texShadow, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texShadow, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//            glTexParameterfv(texShadow, GL_TEXTURE_BORDER_COLOR, borderColor);

            // Set interpolation for texture sampling (GL_NEAREST for no interpolation).
            glTextureParameteri(texShadow, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(texShadow, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // === Create framebuffer for extra texture ===
            GLuint framebuffer;
            glCreateFramebuffers(1, &framebuffer);
            glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, texShadow, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

            // Clear the shadow map and set needed options
            glClearDepth(1.0f);
            glClear(GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            shadowShader.bind();

            glViewport(0, 0, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

            //set the camera to render the shadow map with
            glm::mat4 lightVp;
            lightCamera.getViewProjectionMatrix(lightVp, transform);
            lightVPs.push_back(lightVp);

            //We need to render all meshes from the perspective of the light
            //TODO: this can be optimized if each light knows the meshes it affects
            for (auto entity: view) {
                auto &transform = view.get<TransformComponent>(entity);
                auto &meshRenderer = view.get<MeshRendererComponent>(entity);

                auto worldTransform = transform.worldTransform();
                const glm::mat4 mvpMatrix = lightVp * worldTransform;

                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(worldTransform));

                glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

                meshRenderer.mesh.draw();
            }

            shadowMaps.push_back(texShadow);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }


    void renderMeshes(Shader &debugShader, entt::registry &registry, Entity camera, glm::ivec2 windowSize,
                      float aspectRatio) {

        //Get the lights from the scene
        std::vector<Light> lights;
        std::vector<glm::vec3> lightPos;

        //Get all lights
        auto lightView = registry.view<LightComponent, TransformComponent>();
        // access components with a mesh, localTransform and material
        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        //We are going to be moving this camera around to mimic light directionality
        for (auto lightEntity: lightView) {
            Transform &transform = registry.get<TransformComponent>(lightEntity).localTransform;
            lights.push_back(lightView.get<LightComponent>(lightEntity).light);
            lightPos.push_back(transform.pos);
        }
        clearScreen();

        // Adjust size of window
        Camera *cam = camera.getComponent<CameraComponent>().camera;
        glViewport(0, 0, windowSize.x, windowSize.y);
        cam->updateAspectRatio(aspectRatio);

        // compute view projection matrix
        // TODO get world localTransform properly
        glm::mat4 vpMatrix;
        auto camTransform = camera.getComponent<TransformComponent>().localTransform;
        camera.getComponent<CameraComponent>().camera->getViewProjectionMatrix(vpMatrix, camTransform);

        glEnable(GL_DEPTH_TEST);

        //This is used to access the uniform buffers for the shadow maps and MVP
        int lightCount = lightPos.size();
        int shadowMapBufferOffset = 10 + 2*lightCount;
        int mvpBufferOffset = 10 + 3*lightCount;

        for (auto entity: view) {

            auto &transform = view.get<TransformComponent>(entity);
            auto &meshRenderer = view.get<MeshRendererComponent>(entity);
            auto &materialComponent = view.get<MaterialComponent>(entity);

            auto worldTransform = transform.worldTransform();

            //Actual MVP
            const glm::mat4 mvpMatrix = vpMatrix * worldTransform;

            // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
            // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(worldTransform));

            int texturesUsed = materialComponent.material->bindMaterial(camera.getComponent<TransformComponent>().localTransform.pos,
                                                     lights, lightPos);

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(worldTransform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            //Load shadow map textures and light MVP matrices
            for(int i = 0; i < shadowMaps.size(); i++) {
                int offset = texturesUsed + i;
                glActiveTexture(GL_TEXTURE0 + offset);
                glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
                glUniform1i(shadowMapBufferOffset + i, offset);

                glUniformMatrix4fv(mvpBufferOffset + i, 1, GL_FALSE, glm::value_ptr(lightVPs[i]));
            }

            meshRenderer.mesh.draw();
        }

    }

};
