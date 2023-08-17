#pragma once

#include "framework/entt/entt.h"
#include "../components/RobotArmComponents.h"
#include "../components/Components.h"

class RobotArmSystem {

public:

    void setRotations(entt::registry &registry) {

        auto view = registry.view<TransformComponent, SetRotation>();

        for (auto entity: view) {
            auto &transform = view.get<TransformComponent>(entity);
            auto &data = view.get<SetRotation>(entity);
            transform.transform.rotation[data.axis] = data.rotation;
        }

    }

};
