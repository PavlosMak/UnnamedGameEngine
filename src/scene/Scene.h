#ifndef COMPUTERGRAPHICS_SCENE_H
#define COMPUTERGRAPHICS_SCENE_H

#include "framework/entt/entt.h"
#include "../camera.h"

class Entity;

class Scene : public std::enable_shared_from_this<Scene> {
public:

    Scene();

    Entity createEntity(const std::string &name);

    Entity registerCamera(Camera &camera, glm::mat4 cameraTransform);

    void setup(Camera &camera);

    void update();

private:
    entt::registry m_Registy;
    entt::entity m_MainCamera;

    friend class Entity;

};


#endif //COMPUTERGRAPHICS_SCENE_H
