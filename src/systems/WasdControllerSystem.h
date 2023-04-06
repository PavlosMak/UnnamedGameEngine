#pragma once

#include <iostream>
#include "framework/entt/entt.h"
#include "../scene/Components.h"
#include "glm/ext/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "../components/AnimComponents.h"

class WasdControllerSystem {

    bool wDown = false;
    bool aDown = false;
    bool sDown = false;
    bool dDown = false;
    bool eDown = false;
    bool rDown = false;
    bool oneDown = false;
    bool twoDown = false;
    bool threeDown = false;
    bool rightArrow = false;
    bool leftArrow = false;
    bool upArrow = false;
    bool downArrow = false;

    bool zeroDown = false;
    bool nineDown = false;
    bool eightDown = false;

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
            case GLFW_KEY_1:
                this->oneDown = newState;
                break;
            case GLFW_KEY_2:
                this->twoDown = newState;
                break;
            case GLFW_KEY_3:
                this->threeDown = newState;
                break;
            case GLFW_KEY_RIGHT:
                this->rightArrow = newState;
                break;
            case GLFW_KEY_LEFT:
                this->leftArrow = newState;
                break;
            case GLFW_KEY_UP:
                this->upArrow = newState;
                break;
            case GLFW_KEY_DOWN:
                this->downArrow = newState;
                break;
            case GLFW_KEY_0:
                this->zeroDown = newState;
                break;
            case GLFW_KEY_9:
                this->nineDown = newState;
                break;
            case GLFW_KEY_8:
                this->eightDown = newState;
                break;
        }
    }

    glm::vec3 translationVec(float movementSpeed, Transform &transform) const {

        glm::vec3 unitX = transform.right();
        glm::vec3 unitY = transform.up();
        glm::vec3 unitZ = transform.forward();

        auto translationVec = glm::vec3(0, 0, 0);

        if (aDown) {
            translationVec -= unitX;
        }
        if (dDown) {
            translationVec += unitX;
        }

        if (wDown) {
            translationVec += unitZ;
        }
        if (sDown) {
            translationVec -= unitZ;
        }

        if (eDown) {
            translationVec += unitY;
        }
        if (rDown) {
            translationVec -= unitY;
        }

        return translationVec * movementSpeed;

    }

    glm::vec3 rotationVec(float movementSpeed, Transform &transform) const {

        glm::vec3 unitX = glm::vec3(1.0, 0, 0);
        glm::vec3 unitY = glm::vec3(0, 1, 0);
        glm::vec3 unitZ = transform.forward();

        auto rotationVec = glm::vec3(0, 0, 0);

        if (rightArrow) {
            rotationVec -= unitY;
        }
        if (leftArrow) {
            rotationVec += unitY;
        }

        return 100.f * rotationVec * movementSpeed;
    }

public:

    void onKeyPressed(int key) {
        changeKeyState(key, true);
    }

    void onKeyReleased(int key) {
        changeKeyState(key, false);

    }

    void updateCam(entt::registry &registry) const {

        auto view = registry.view<CamControllerComp, TransformComponent>();

        for (auto entity: view) {
            auto &controller = view.get<CamControllerComp>(entity);
            auto &transformC = view.get<TransformComponent>(entity);

            if (zeroDown) {
                controller.viewIdx = 0;
            } else if (nineDown) {
                controller.viewIdx = 1;
            } else if (eightDown) {
                controller.viewIdx = 2;
            }

            if (controller.viewIdx == 1) {

                // fps
                auto newTransform = *controller.target;
                newTransform.pos += newTransform.forward() * 0.45f;
                transformC.transform = newTransform;

            } else if (controller.viewIdx == 0) {

                // back view
                auto newTransform = *controller.target;
                newTransform.pos += newTransform.forward() * - 1.f;
                newTransform.pos.y += 0.3f;
                transformC.transform = newTransform;

            } else if (controller.viewIdx == 2) {

                // front view
                auto newTransform = *controller.target;
                newTransform.rotation.y = 185;
                newTransform.pos -= newTransform.forward() * 1.f;
                newTransform.pos.y += 0.3f;

                transformC.transform = newTransform;
            }
        }
    }

    void update(entt::registry &registry) const {

        updateCam(registry);

        auto view = registry.view<WasdComponent, TransformComponent>();

        for (auto entity: view) {
            auto &wasdC = view.get<WasdComponent>(entity);
            auto &transformC = view.get<TransformComponent>(entity);

            // update position and rotation
            transformC.transform.pos += translationVec(wasdC.movementSpeed, transformC.transform);
            transformC.transform.rotation += rotationVec(wasdC.movementSpeed, transformC.transform);
        }

        auto puzzleView = registry.view<PuzzleObjectComponent, MaterialComponent, TransformComponent>();

        auto player = registry.view<PlayerComponent>()[0];
        auto &playerTransform = registry.get<TransformComponent>(player);

        //That the distance to register input
        float range = 0.3;

        auto baseAnim = Anim(true, 3);
        auto rotY = [](Transform trans, float t) {
            auto newTransform(trans);
            newTransform.rotation = glm::vec3(0, 360 * t, 0);
            return newTransform;
        };

        for (auto entity: puzzleView) {
            auto &puzzle = puzzleView.get<PuzzleObjectComponent>(entity);
            auto &mat = puzzleView.get<MaterialComponent>(entity);
            auto &trans = puzzleView.get<TransformComponent>(entity);
            auto &tag = registry.get<TagComponent>(entity);

            float distToPlayer = glm::distance(playerTransform.transform.pos, trans.transform.pos);
//            bool inRange = 0.9<= distToPlayer && distToPlayer <= 1.2;
            bool inRange = true;
            if (puzzle.solved) {
                continue;
            }
            if (oneDown && inRange) {
                mat.material = puzzle.material0;
                puzzle.currentActive = 0;
            } else if (twoDown && inRange) {
                mat.material = puzzle.material1;
                puzzle.currentActive = 1;
            } else if (threeDown && inRange) {
                mat.material = puzzle.material2;
                puzzle.currentActive = 2;
            }
            if (puzzle.currentActive == puzzle.key) {
                puzzle.solved = true;
                registry.emplace<TransformAnimation>(entity, baseAnim, rotY);
            }
        }
    }
};
