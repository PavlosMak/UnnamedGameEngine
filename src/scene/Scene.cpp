#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "../materials/MaterialManager.h"

Scene::Scene(entt::registry &registry) : m_registry(registry), m_shaderManager() {}

SceneStats Scene::getSceneStats() {
    return m_stats;
}

Entity Scene::getEntityByTag(std::basic_string<char> tag) {
    return Entity{m_tagToEntity[tag], m_registry};
}

void Scene::updateStatistics() {
    auto group = m_registry.group<MeshRendererComponent, TransformComponent>();
    for (auto entity: group) {
        MeshRendererComponent &meshRenderer = group.get<MeshRendererComponent>(entity);
        m_stats.triangleCount += meshRenderer.mesh.getTriangleCount();
    }
}

void Scene::setup(Camera &camera) {
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/solid_color_frag.glsl", SHADER_TYPE::SOLID_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/shader_frag.glsl", SHADER_TYPE::NORMAL_AS_COLOR);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/phong_frag.glsl", SHADER_TYPE::PHONG);
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_frag.glsl", SHADER_TYPE::PBR);

    MaterialManager* materialManager = MaterialManager::getInstance();

    //TODO: We probably want a material pool to access them by name
//    Material pbrMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec3(1, 0, 0), 0.5f, .5f);

    std::shared_ptr<Material> pbrMaterial = materialManager->createPBRMaterial(m_shaderManager.getShader(SHADER_TYPE::PBR), glm::vec3(1,0,0), 0.5f, 0.5f);
    auto light = Light(glm::vec3(1.0f));

    Entity light1 = this->createEntity("Light1");
    light1.addComponent<LightComponent>(light);
    light1.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1.5,0.339,0.25)));

    Entity light2 = this->createEntity("Light2");
    light2.addComponent<LightComponent>(light);
    light2.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1.5,-0.244,0.25)));

    Entity light3 = this->createEntity("Light3");
    light3.addComponent<LightComponent>(light);
    light3.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1.5,-0.244,-0.168)));

    Entity light4 = this->createEntity("Light4");
    light4.addComponent<LightComponent>(light);
    light4.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1.5,0.339,-0.168)));

    Entity sphere = this->createEntity("Sphere");
    sphere.addComponent<MeshRendererComponent>("resources/sphere.obj");
    sphere.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0, 0, 0)));
    sphere.addComponent<MaterialComponent>(pbrMaterial);
    sphere.addComponent<WasdComponent>(0.1f);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-4, 0, 0)));
    cameraEntity.addComponent<CameraComponent>(camera, glm::vec3(0.0f));

    updateStatistics();
}

void Scene::update(const long long &timeStep) {
}

Entity Scene::createEntity(const std::string &name) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}

