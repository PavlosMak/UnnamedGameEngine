#include "Scene.h"
#include "../managers/MaterialManager.h"
#include "Entity.h"
#include "../components/Components.h"
#include "../components/AnimComponents.h"

#include "fstream"

Scene::Scene(const std::string& pathToFile) {
    //Parse the file at the scene creation, so we don't need to read the file when setting up
    //(this might create an issue for many scenes)
    std::ifstream file(pathToFile);
    this->m_sceneData = nlohmann::json::parse(file);
}

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

void Scene::setup() {
    std::vector<nlohmann::json> entities = this->m_sceneData["entities"].get<nlohmann::json>();

    for(const auto& entityData : entities) {
        this->createEntity(entityData);
    }

    updateStatistics();
}

void Scene::update(const long long &timeStep) {

}

Entity Scene::createEntity(const nlohmann::json &entityData) {
    //Setup basic entity properties
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    std::string name = entityData["name"].get<std::string>();
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    //Setup transform
    if(entityData.contains("transform")) {
        Transform transform = Transform(entityData["transform"]);
        entity.addComponent<TransformComponent>(transform.pos, transform.rotation, transform.scale);
    } else {
        entity.addComponent<TransformComponent>();
    }
    //Add any light
    if(entityData.contains("light")) {
        //TODO This is not really nice. Should rethink lights
        std::vector<float> intensity = entityData["light"].get<std::vector<float>>();
        entity.addComponent<LightComponent>(glm::vec3(intensity[0], intensity[1], intensity[2]));
        int roomId = entityData["room"].get<int>(); //TODO Remnant of old optimization, will be removed
        entity.addComponent<RoomComponent>(roomId);
    }

    if(entityData.contains("mesh")) {
        //TODO Currently absolute path, need a better way to handle that
        std::string meshPath = entityData["mesh"].get<std::string>();
        entity.addComponent<MeshRendererComponent>(meshPath);
    }
    if(entityData.contains("material")) {
        std::string materialName = entityData["material"].get<std::string>();
        Material* material = MaterialManager::getInstance()->getMaterialByName(materialName);
        entity.addComponent<MaterialComponent>(material);
    }

    if(entityData.contains("camera")) {
        entity.addComponent<CameraComponent>(&m_camera);
    }
    if(entityData.contains("wasd")) {
        float wasdValue = entityData["wasd"].get<float>();
        entity.addComponent<WasdComponent>(wasdValue);
    }
    if(entityData.contains("player")) {
        entity.addComponent<PlayerComponent>();
    }
    return entity;
}

Entity Scene::createEntityParented(const std::string &name, Entity &parent, Transform t) {
    auto handle = m_registry.create();
    Entity entity = {handle, m_registry};
    entity.addComponent<TagComponent>(name);
    m_tagToEntity.emplace(name, handle);
    auto parentTransform = &parent.getComponent<TransformComponent>().transform;
    entity.addComponent<TransformComponent>(t.pos, t.rotation, t.scale, parentTransform);
    return entity;
}
