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
    m_shaderManager.loadShader("shaders/shader_vert.glsl", "shaders/pbr_textured_frag.glsl", SHADER_TYPE::TEXTURED_PBR);

    MaterialManager* materialManager = MaterialManager::getInstance();

    Material* texturedMaterial = materialManager->createTexturedPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::TEXTURED_PBR),
            "resources/dragon-scales/normal.png",
            "resources/dragon-scales/roughness.png",
            "resources/dragon-scales/metallic.png",
            "resources/dragon-scales/albedo.png",
            "resources/dragon-scales/ao.png",
            "resources/dragon-scales/height.png");

    Material* mat2 = materialManager->createTexturedPBRMaterial(
            m_shaderManager.getShader(SHADER_TYPE::TEXTURED_PBR),
            "resources/bricks/normal.png",
            "resources/bricks/roughness.png",
            "resources/bricks/metallic.png",
            "resources/bricks/albedo.png",
            "resources/bricks/ao.png",
            "resources/bricks/height.png");

    auto light = Light(glm::vec3(10.0f));

    //Define some lights
    Entity light1 = this->createEntity("Light1");
    light1.addComponent<LightComponent>(light);
    light1.addComponent<TransformComponent>(glm::vec3(0.226, .860, -1.031), glm::vec3(0,240,0), glm::vec3(1));

////
//    Entity light2 = this->createEntity("Light2");
//    light2.addComponent<LightComponent>(light);
//    light2.addComponent<TransformComponent>(glm::vec3(-1.5,-0.244,0.25), glm::vec3(0), glm::vec3(1));
//
//    Entity light3 = this->createEntity("Light3");
//    light3.addComponent<LightComponent>(light);
//    light3.addComponent<TransformComponent>(glm::vec3(-1.5,-0.244,-0.168), glm::vec3(0), glm::vec3(1));
//
//    Entity light4 = this->createEntity("Light4");
//    light4.addComponent<LightComponent>(light);
//    light4.addComponent<TransformComponent>(glm::vec3(-1.5,0.339,-0.168), glm::vec3(0), glm::vec3(1));

    Entity smaug = this->createEntity("Smaug");
    smaug.addComponent<MeshRendererComponent>("resources/dragon.obj");
    smaug.addComponent<TransformComponent>(glm::vec3(1.573, 0, 0), glm::vec3(0), glm::vec3(1));
    smaug.addComponent<MaterialComponent>(texturedMaterial);

    Entity mushu = this->createEntity("Mushu");
    mushu.addComponent<MeshRendererComponent>("resources/dragon.obj");
    mushu.addComponent<TransformComponent>(glm::vec3(2.103, -0.030, -0.350), glm::vec3(0, -20,0), glm::vec3(1));
    mushu.addComponent<MaterialComponent>(texturedMaterial);

    Entity model2 = this->createEntity("Ground");
    model2.addComponent<MeshRendererComponent>("resources/cube.obj");
    model2.addComponent<TransformComponent>(glm::vec3(1.573, -1.270, 0), glm::vec3(0), glm::vec3(5.900, 0.995, 4.840));
    model2.addComponent<MaterialComponent>(mat2);

    Entity cameraEntity = this->createEntity("Camera");
    cameraEntity.addComponent<TransformComponent>(glm::vec3(0, 0, -1), glm::vec3(0, 240, 0), glm::vec3(1));
    cameraEntity.addComponent<CameraComponent>(&camera);
    cameraEntity.addComponent<WasdComponent>(0.02f);

    updateStatistics();
}

void Scene::update(const long long &timeStep) {}

Entity Scene::createEntity(const std::string &name) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    return entity;
}

