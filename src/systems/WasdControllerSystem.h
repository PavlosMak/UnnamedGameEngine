#pragma once

#include <iostream>
#include "framework/entt/entt.h"
#include "../scene/Components.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"

class WasdControllerSystem {

    bool wDown = false;
    bool aDown = false;
    bool sDown = false;
    bool dDown = false;
    bool eDown = false;
    bool rDown = false;

    void changeKeyState(int key, bool newState) {
        switch (key) {
            case GLFW_KEY_A:
                this->aDown = newState;
                break;
            case GLFW_KEY_W:
                this->wDown = newState;
                break;
            case GLFW_KEY_S:
                this->sDown = newState;
                break;
            case GLFW_KEY_D:
                this->dDown = newState;
                break;
            case GLFW_KEY_E:
                this->eDown = newState;
                break;
            case GLFW_KEY_R:
                this->rDown = newState;
                break;
        }
    }

    glm::vec3 translationVec(float movementSpeed) const {

        glm::vec3 unitX = glm::vec3(movementSpeed, 0, 0);
        glm::vec3 unitY = glm::vec3(0, movementSpeed, 0);
        glm::vec3 unitZ = glm::vec3(0, 0, movementSpeed);

        auto translationVec = glm::vec3(0, 0, 0);

        if (aDown) {
            translationVec += unitZ;
        } if (dDown) {
            translationVec -= unitZ;
        }

        if (wDown) {
            translationVec += unitX;
        } if (sDown) {
            translationVec -= unitX;
        }

        if (eDown) {
            translationVec -= unitY;
        } if (rDown) {
            translationVec += unitY;
        }

        return translationVec;

    }

public:

    void onKeyPressed(int key) {
        changeKeyState(key, true);
    }

    void onKeyReleased(int key) {
        changeKeyState(key, false);
    }

    void update(entt::registry& registry) const {
        auto view = registry.view<WasdComponent, TransformComponent>();

        for (auto entity : view) {
            auto &wasdC = view.get<WasdComponent>(entity);
            auto &transformC = view.get<TransformComponent>(entity);

            // update position
            transformC.pos += translationVec(wasdC.movementSpeed);
        }

    }
};
