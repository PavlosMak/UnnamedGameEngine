#pragma once

#include "framework/entt/entt.h"
#include "unordered_map"

#include "../camera.h"
#include "../materials/ShaderManager.h"
#include "../lights/Light.h"
#include "../materials/Material.h"
#include "string"

#include "framework/json.hpp"


class Entity;

struct SceneStats {
    int triangleCount = 0;
};

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene(const std::string& pathToFile);

    Entity createEntity(const nlohmann::json &entityData);

    Entity createEntityParented(const std::string &name, Entity &parent, Transform t);

    //This implies that we use unique tags, we might want to rename this to something like ID.
    Entity getEntityByTag(std::basic_string<char> tag);

    void setup();

    void update(const long long &timeStep);

    SceneStats getSceneStats();

    entt::registry m_registry;

private:

    void updateStatistics();

    Camera m_camera;
    nlohmann::json m_sceneData;

    std::string m_name;
    std::vector<Light> lights;
    ShaderManager m_shaderManager;
    std::unordered_map<std::string, entt::entity> m_tagToEntity;
    SceneStats m_stats;


    friend class Entity;
};

