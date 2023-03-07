#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

Scene::Scene() = default;


Entity Scene::getEntityByTag(std::basic_string<char> tag) {
    return Entity{m_tagToEntity[tag], this};
}

void Scene::setup(Camera &camera) {

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1,0.3,-1)));
    cameraEntity.addComponent<CameraComponent>(camera, glm::vec3(0.0f));

    Entity secondDragon = this->createEntity("Syrax");
    secondDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
    secondDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, 1.0)));

    Entity thirdDragon = this->createEntity("Mushu");
    thirdDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
    thirdDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, -1.0)));

    Entity ape = this->createEntity("Ape");
    ape.addComponent<MeshRendererComponent>("resources/ape.obj");
    ape.addComponent<TransformComponent>(glm::scale(
            glm::rotate(glm::mat4{1.0f},
                        glm::radians(-45.f),
                        glm::vec3(0.0, 1.0, 0.0)),
            glm::vec3(0.1, 0.1, 0.1))
    );
}

void Scene::update() {
    glm::mat4 vpMatrix;

    Entity camera = this->getEntityByTag("Camera");
    auto &mainCameraComponent = camera.getComponent<CameraComponent>();
    auto &mainCameraTransform = camera.getComponent<TransformComponent>();
    mainCameraComponent.getViewProjectionMatrix(vpMatrix, mainCameraTransform.transform);

    auto group = m_Registy.group<MeshRendererComponent, TransformComponent>();
    for (auto entity: group) {
        TransformComponent &transform = group.get<TransformComponent>(entity);
        MeshRendererComponent &meshRenderer = group.get<MeshRendererComponent>(entity);

        const glm::mat4 mvpMatrix = vpMatrix * transform.transform;
        // Normals should be transformed differently than positions (ignoring translations + dealing with scaling):
        // https://paroj.github.io/gltut/Illumination/Tut09%20Normal%20Transformation.html
        const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(transform.transform));

        glEnable(GL_DEPTH_TEST);

        glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
        glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(transform.transform));
        glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(normalModelMatrix));

        meshRenderer.mesh.draw();

        glUniform1i(4, GL_FALSE);
    }
}


Entity Scene::createEntity(const std::string &name) {
    auto handle = m_Registy.create();
    Entity entity = {handle, this};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}