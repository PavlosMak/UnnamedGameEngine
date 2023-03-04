#ifndef COMPUTERGRAPHICS_ENTITY_H
#define COMPUTERGRAPHICS_ENTITY_H

#include "Scene.h"
#include "framework/entt/entt.h"

class Entity {
public:
    Entity(entt::entity handle, Scene *scene);


    /**
     * Creates a new component in the entity. We are using
     * parameter forwarding instead of passing an already created
     * constructor for performance.
     * @tparam T The type of the component we are adding.
     * @tparam Args The arguments for the components constructor.
     * @return A reference to the added component.
     */
    template<typename T>
    bool hasComponent() {
        return m_Scene->m_Registy.any_of<T>(m_EntityHandle);
    }

    template<typename T, typename... Args>
    T &addComponent(Args &&... args) {
        assert(!hasComponent<T>());
        return m_Scene->m_Registy.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T &getComponent() {
        assert(hasComponent<T>());
        return m_Scene->m_Registy.get<T>(m_EntityHandle);
    }

    template<typename T>
    void removeComponent() {
        assert(hasComponent<T>());
        m_Scene->m_Registy.remove<T>(m_EntityHandle);
    }

private:
    entt::entity m_EntityHandle;
    Scene *m_Scene;
};


#endif //COMPUTERGRAPHICS_ENTITY_H
