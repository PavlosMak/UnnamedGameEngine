#include "Entity.h"

#include <utility>

Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(std::move(scene)) {}
