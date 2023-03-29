#pragma once

#include "../scene/Components.h"
#include "framework/entt/entt.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../scene/Entity.h"

class RenderSystem {

private:
    glm::mat4 m_cameraVP;
    Camera m_lightCamera = Camera(); //Camera to render shadow maps with

    std::vector<Light> lights;
    std::vector<glm::vec3> lightPos;
    std::vector<Transform> lightTransforms;
    std::vector<GLuint> shadowMaps;
    std::vector<glm::mat4> lightVPs;


    //This is used to access the uniform buffers for the shadow maps and MVP
    int m_shadowMapBufferOffset;
    int m_mvpBufferOffset;

    GLuint m_Shadowframebuffer;

    //Dimensions for shadow maps
    const int SHADOWTEX_WIDTH = 1024;
    const int SHADOWTEX_HEIGHT = 1024;

    inline void clearScreen() {
        // Clear the screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void renderInShadowMap(Shader& shadowShader,
                           entt::registry& registry,
                           GLuint& texShadow,
                           glm::mat4& lightVp, auto view) {
        glNamedFramebufferTexture(m_Shadowframebuffer, GL_DEPTH_ATTACHMENT, texShadow, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, m_Shadowframebuffer);

        // Clear the shadow map and set needed options
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

        //We need to render all meshes from the perspective of the light
        //TODO: this can be optimized if each light knows the meshes it affects
        for (auto entity: view) {
            auto &meshTransform = view.template get<TransformComponent>(entity);
            auto &meshRenderer = view.template get<MeshRendererComponent>(entity);

            auto worldTransform = meshTransform.worldTransform();
            const glm::mat4 mvpMatrix = lightVp * worldTransform;

            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(worldTransform));

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

            meshRenderer.mesh.draw();
        }

        shadowMaps.push_back(texShadow);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

public:

    void init(Shader &shadowShader, entt::registry& registry) {
        //Get all lights
        auto lightView = registry.view<LightComponent, TransformComponent>();

        //We are going to be moving this camera around to mimic light directionality
        for (auto lightEntity: lightView) {
            Transform &transform = registry.get<TransformComponent>(lightEntity).localTransform;
            lights.push_back(lightView.get<LightComponent>(lightEntity).light);
            lightPos.push_back(transform.pos);
            lightTransforms.push_back(transform);
        }

        //This is used to access the uniform buffers for the shadow maps and MVP
        int lightCount = lightPos.size();
        m_shadowMapBufferOffset = 10 + 2*lightCount;
        m_mvpBufferOffset = 10 + 3*lightCount;

        createShadowMaps(shadowShader, registry);
    }

    void createShadowMaps(Shader &shadowShader, entt::registry &registry) {
        //Create the framebuffer that renders shadow maps
        glCreateFramebuffers(1, &m_Shadowframebuffer);

        //Bind the shadow shader once
        shadowShader.bind();

        for (auto transform: lightTransforms) {
            // === Create Shadow Texture ===
            GLuint texShadow;
            glCreateTextures(GL_TEXTURE_2D, 1, &texShadow);
            glTextureStorage2D(texShadow, 1, GL_DEPTH_COMPONENT32F, SHADOWTEX_WIDTH, SHADOWTEX_HEIGHT);

            // Set behaviour for when texture coordinates are outside the [0, 1] range.
            glTextureParameteri(texShadow, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(texShadow, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Set interpolation for texture sampling (GL_NEAREST for no interpolation).
            glTextureParameteri(texShadow, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(texShadow, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            //set the camera to render the shadow map with
            glm::mat4 lightVp;
            m_lightCamera.getViewProjectionMatrix(lightVp, transform);
            lightVPs.push_back(lightVp);

            // access components with a mesh, localTransform and material
            auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

            renderInShadowMap(shadowShader, registry, texShadow, lightVp, view);
        }
    }


    inline void updateShadowMaps(Shader& shadowShader, entt::registry& registry, auto view) {
        auto lightView = registry.view<LightComponent, TransformComponent>();
        int lightIndex = 0;
        //We are going to be moving this camera around to mimic light directionality
        glm::mat4 lightVp;
        for (auto lightEntity: lightView) {
            Transform &transform = registry.get<TransformComponent>(lightEntity).localTransform;
            lights[lightIndex] = lightView.get<LightComponent>(lightEntity).light;
            lightPos[lightIndex] = transform.pos;
            lightTransforms[lightIndex] = transform;
            m_lightCamera.getViewProjectionMatrix(lightVp, transform);
            lightVPs[lightIndex] = lightVp;
            renderInShadowMap(shadowShader, registry, shadowMaps[lightIndex], lightVp, view);
            lightIndex++;
        }
//        for(int i = 0; i < lightTransforms.size(); i++) {
//            GLuint& texShadow = shadowMaps[i];
//            glm::mat4& lightVp = lightVPs[i];
//            renderInShadowMap(shadowShader, registry, texShadow, lightVp, view);
//        }
    }

    void renderMeshes(Shader& shadowShader, entt::registry &registry, Entity camera, glm::ivec2 windowSize,
                      float aspectRatio) {
        clearScreen();

        // access components with a mesh, localTransform and material
        auto view = registry.view<MeshRendererComponent, TransformComponent, MaterialComponent>();

        updateShadowMaps(shadowShader,registry, view);

        // Adjust size of window
        Camera *cam = camera.getComponent<CameraComponent>().camera;
        glViewport(0, 0, windowSize.x, windowSize.y);
        cam->updateAspectRatio(aspectRatio);

        // compute view projection matrix
        // TODO get world localTransform properly
        auto& camTransform = camera.getComponent<TransformComponent>().localTransform;
        camera.getComponent<CameraComponent>().camera->getViewProjectionMatrix(m_cameraVP, camTransform);

        glEnable(GL_DEPTH_TEST);

        //Some state used to switch shaders around
        int prevMaterial = -1;
        int texturesUsed = 0;

        for (auto entity: view) {

            auto &transform = view.get<TransformComponent>(entity);
            auto &meshRenderer = view.get<MeshRendererComponent>(entity);
            auto &materialComponent = view.get<MaterialComponent>(entity);

            auto worldTransform = transform.worldTransform();

            //Actual MVP
            const glm::mat4 mvpMatrix = m_cameraVP * worldTransform;
            // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
            // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
            const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(worldTransform));

            //Bind material only if it's new
            if(prevMaterial != materialComponent.material->ID) {
                texturesUsed = materialComponent.material->bindMaterial(camera.getComponent<TransformComponent>().localTransform.pos,lights, lightPos);
                prevMaterial = materialComponent.material->ID;
            }

            glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
            glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(worldTransform));
            glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

            //Load shadow map textures and light MVP matrices
            for(int i = 0; i < shadowMaps.size(); i++) {
                int offset = texturesUsed + i;
                glActiveTexture(GL_TEXTURE0 + offset);
                glBindTexture(GL_TEXTURE_2D, shadowMaps[i]);
                glUniform1i(m_shadowMapBufferOffset + i, offset);
                glUniformMatrix4fv(m_mvpBufferOffset + i, 1, GL_FALSE, glm::value_ptr(lightVPs[i]));
            }

            meshRenderer.mesh.draw();
        }

    }

};
