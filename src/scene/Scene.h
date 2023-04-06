#pragma once

#include "framework/entt/entt.h"
#include "unordered_map"

#include "../camera.h"
#include "../materials/ShaderManager.h"
#include "../lights/Light.h"
#include "../materials/Material.h"

class Entity;

struct SceneStats {
    int triangleCount = 0;
};

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene(entt::registry &registry);

    Entity createEntity(const std::string &name);

    Entity createEntityParented(const std::string &name, Entity &parent, Transform t);

    //This implies that we use unique tags, we might want to rename this to something like ID.
    Entity getEntityByTag(std::basic_string<char> tag);

    void setup(Camera &camera);

    void update(const long long &timeStep);

    SceneStats getSceneStats();

    std::vector<entt::entity> animationRoomEntities;
    std::vector<entt::entity> mainRoomEntities;
    std::vector<entt::entity> spotLightRoom;

private:

    void updateStatistics();

    std::vector<Light> lights;
    ShaderManager m_shaderManager;
    entt::registry &m_registry;
    std::unordered_map<std::string, entt::entity> m_tagToEntity;
    SceneStats m_stats;

    friend class Entity;

    Entity loadRobotArm(Entity &parent, Transform t, Material *mat);

    Entity decoratePlayer(Entity& adam, Material *matAdam, Material* matEyes, Material* matLens, Material* matRed);

    Entity loadPedestal(Entity &parent, Transform t, Material *pedestalMat, Material *meshMat,
                        Material *meshMat2,
                        Material *meshMat3,
                        int key, std::string mesh);

    Entity loadScene(Material *matGround, Material *matWalls, Material *matArches);
};

