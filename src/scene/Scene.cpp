#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"

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
    //TODO: We probably want a material pool to access them by name
    Material material = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(1.0, 0.0, 0.0),
                                 SHADER_TYPE::SOLID_COLOR);
    Material materialBlue = Material(m_shaderManager.getShader(SHADER_TYPE::SOLID_COLOR), glm::vec3(0.0, 1.0, 0.0),
                                     SHADER_TYPE::SOLID_COLOR);
    Material normalMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::NORMAL_AS_COLOR),
                                       SHADER_TYPE::NORMAL_AS_COLOR);
    Material phongMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::PHONG),
                                      glm::vec3(.8, 0, 0), 1,
                                      SHADER_TYPE::PHONG);
    Material pbrMaterial = Material(m_shaderManager.getShader(SHADER_TYPE::PBR),
                                      glm::vec3(.8, 0, 0), 1,
                                      SHADER_TYPE::PBR);

    auto light = Light(glm::vec3(1.0f));

    Entity light1 = this->createEntity("Light1");
    light1.addComponent<LightComponent>(light);
    light1.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1,1,1)));

    Entity light2 = this->createEntity("Light2");
    light2.addComponent<LightComponent>(light);
    light2.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-1,1,-1)));

    Entity sphere = this->createEntity("Sphere");
    sphere.addComponent<MeshRendererComponent>("resources/sphere.obj");
    sphere.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(0, 0, 0)));
    sphere.addComponent<MaterialComponent>(pbrMaterial);
    sphere.addComponent<WasdComponent>(0.1f);

//    Entity batman = this->createEntity("Batman");
//    batman.addComponent<MeshRendererComponent>("resources/batman.obj");
//    batman.addComponent<TransformComponent>(glm::scale(glm::rotate(
//            glm::translate(glm::mat4{1.0f}, glm::vec3(0,.3,0.0)), glm::degrees(90.0f),glm::vec3(0, 1, 0)), glm::vec3(0.3, 0.3, 0.3)));
//    batman.addComponent<MaterialComponent>(phongMaterial);
//        Entity sponza = this->createEntity("Sponza");
//    //    //TODO: The app loads very slowly if we use nested path for the resource
//        sponza.addComponent<MeshRendererComponent>("resources/sponza.obj");
//        sponza.addComponent<TransformComponent>(glm::scale(glm::mat4{1.0f}, glm::vec3(0.02, 0.02, 0.02)));
//        sponza.addComponent<MaterialComponent>(phongMaterial);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::translate(glm::mat4{1.0f}, glm::vec3(-2, 0, 0)));
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

