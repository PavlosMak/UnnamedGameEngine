#include "Scene.h"
#include "Entity.h"
#include "Components.h"

Scene::Scene() {
}


Entity Scene::createEntity(const std::string &name) {
    Entity entity = {m_Registy.create(), this};
    entity.addComponent<TagComponent>(name);
    return entity;
}