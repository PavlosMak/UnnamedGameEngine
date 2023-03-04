#ifndef COMPUTERGRAPHICS_SCENE_H
#define COMPUTERGRAPHICS_SCENE_H

#include "framework/entt/entt.h"


class Entity;

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene();

    Entity createEntity(const std::string &name);

private:
    entt::registry m_Registy;

    friend class Entity;

};


#endif //COMPUTERGRAPHICS_SCENE_H
