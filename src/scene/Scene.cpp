#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

Scene::Scene(entt::registry &registry) : m_registry(registry), m_shaderManager() {}

Entity Scene::getEntityByTag(std::basic_string<char> tag) {
    return Entity{m_tagToEntity[tag], m_registry};
}

void Scene::setup(Camera &camera) {
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/solid_color_frag.glsl", SHADER_TYPE::SOLID_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl", SHADER_TYPE::NORMAL_AS_COLOR);
    //TODO: We probably want a material pool to access them by name
    Material material = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(1.0, 0.0, 0.0),
                                 SHADER_TYPE::SOLID_COLOR);
    Material materialBlue = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(0.0, 1.0, 0.0),
                                     SHADER_TYPE::SOLID_COLOR);
    Material normalMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::NORMAL_AS_COLOR),
                                       SHADER_TYPE::NORMAL_AS_COLOR);

//    Entity ape = this->createEntity("Ape");
//    ape.addComponent<MeshRendererComponent>("resources/ape.obj");
//    ape.addComponent<TransformComponent>(glm::scale(glm::rotate(glm::mat4{1.0f},
//                                                                glm::radians(-45.f),
//                                                                glm::vec3(0.0, 1.0, 0.0)),
//                                                    glm::vec3(0.1, 0.1, 0.1))
//    );
//    ape.addComponent<MaterialComponent>(normalMaterial);

    Entity secondDragon = this->createEntity("Syrax");
    secondDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
    secondDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, 1.0)));
    secondDragon.addComponent<MaterialComponent>(normalMaterial);

    Entity thirdDragon = this->createEntity("Mushu");
    thirdDragon.addComponent<MeshRendererComponent>("resources/dragon.obj");
    thirdDragon.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0.0, 0.0, -1.0)));
    thirdDragon.addComponent<MaterialComponent>(normalMaterial);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1, 0.3, -1)));
    cameraEntity.addComponent<CameraComponent>(camera, glm::vec3(0.0f));
    cameraEntity.addComponent<WasdComponent>(0.1f);

    Entity cube = this->createEntity("Cube");
    cube.addComponent<MeshRendererComponent>("resources/cube.obj");
    cube.addComponent<SRTTransformComponent>(glm::vec3(0), glm::vec3(0), glm::vec3(0.1, 0.1, 0.1));
    cube.addComponent<MaterialComponent>(normalMaterial);

    Entity bicep = this->createEntity("Bicep");
    bicep.addComponent<MeshRendererComponent>("resources/cube.obj");
    bicep.addComponent<SRTTransformComponent>(glm::vec3(0), glm::vec3(0), glm::vec3(0.8, 2, 0.8), &cube.getComponent<SRTTransformComponent>());
    bicep.addComponent<MaterialComponent>(normalMaterial);

}

void Scene::update(const long long &timeStep) {}

Entity Scene::createEntity(const std::string &name) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}

