#ifndef COMPUTERGRAPHICS_SCENE_H
#define COMPUTERGRAPHICS_SCENE_H

#include "framework/entt/entt.h"
#include "unordered_map"

#include "../camera.h"

class Entity;

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene();

    Entity createEntity(const std::string &name);

    //This implies that we use unique tags, we might want to rename this to something like id.
    Entity getEntityByTag(std::basic_string<char> tag);

    void setup(Camera &camera);

    void update();

private:
    entt::registry m_Registy;
//    entt::entity m_MainCamera;
    std::unordered_map<std::string,entt::entity> m_tagToEntity;

    friend class Entity;

};


#endif //COMPUTERGRAPHICS_SCENE_H
