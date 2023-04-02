#pragma once

#include "math.h"
#include "framework/entt/entt.h"
#include "../components/AnimComponents.h"
#include "../scene/Components.h"

class AnimationSystem {

public:
    void stepAnimations(entt::registry &registry, double dt) {

        auto view = registry.view<TransformComponent, BezierAnimation>();

        for (auto entity: view) {
            auto &transform = view.get<TransformComponent>(entity);
            auto &anim = view.get<BezierAnimation>(entity);

            if (anim.anim.playing) {
                float t = anim.anim.nextT(dt);

                // set the position with the t
                glm::vec3 pos = anim.curve.eval(t);
                transform.transform.pos = pos;

                anim.anim.prevT = t;
            }

        }

        auto viewRot = registry.view<TransformComponent, RotationAnimation>();

        for (auto entity: viewRot) {
            auto &transform = viewRot.get<TransformComponent>(entity);
            auto &anim = viewRot.get<RotationAnimation>(entity);

            if (anim.anim.playing) {
                float t = anim.anim.nextT(dt);

                //glm::vec3 a = anim.rotationFun(t);
                transform.transform.rotation = anim.rotationFun(t);

                anim.anim.prevT = t;
            }

        }

        auto viewTrans = registry.view<TransformComponent, TransformAnimation>();

        for (auto entity: viewTrans) {
            auto &transform = viewTrans.get<TransformComponent>(entity);
            auto &anim = viewTrans.get<TransformAnimation>(entity);

            if (anim.anim.playing) {
                float t = anim.anim.nextT(dt);

                // evaluate callback
                transform.transform = anim.callback(transform.transform, t);

                anim.anim.prevT = t;
            }
        }


    }
};
