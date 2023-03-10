#ifndef COMPUTERGRAPHICS_SCENE_H
#define COMPUTERGRAPHICS_SCENE_H

#include "framework/entt/entt.h"
#include "unordered_map"

#include "../camera.h"
#include "../materials/ShaderManager.h"
#include "../lights/Light.h"

class Entity;

struct SceneStats {
    int triangleCount = 0;
};

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene();

    Entity createEntity(const std::string &name);

    //This implies that we use unique tags, we might want to rename this to something like ID.
    Entity getEntityByTag(std::basic_string<char> tag);

    void setup(Camera &camera);

    void update(const long long &timeStep);

    SceneStats getSceneStats();

private:
    void updateStatistics();

    std::vector<Light> lights;
    ShaderManager m_shaderManager;
    entt::registry m_Registy;
    std::unordered_map<std::string, entt::entity> m_tagToEntity;
    SceneStats m_stats;

    friend class Entity;

};


#endif //COMPUTERGRAPHICS_SCENE_H
