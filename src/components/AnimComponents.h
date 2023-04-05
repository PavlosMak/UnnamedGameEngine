#pragma once

#include <utility>

#include "glm/vec3.hpp"

class CubicBezier {
public:
    glm::vec3 point1;
    glm::vec3 handle1;
    glm::vec3 point2;
    glm::vec3 handle2;

    CubicBezier() = default;

    CubicBezier(glm::vec3 p0, glm::vec3 h0, glm::vec3(p1), glm::vec3(h1))
            : point1(p0), handle1(h0), point2(p1), handle2(h1) {}

    glm::vec3 eval(float t) const {
        float tm = 1 - t;
        return tm * tm * tm * point1 + 3 * tm * tm * t * handle1 + 3 * tm * t * t * point2 + t * t * t * handle2;
    }
};


class Anim {
public:

    Anim() = default;

    Anim(bool playing, float duration) :
            playing(playing), prevT(0.0f), duration(duration) {}

    bool playing;
    float prevT;
    float duration;

    float nextT(double dt) {
        float t = prevT + (dt / this->duration);

        if (t > 1.0f) {
            t = 0.0f;
        }

        return t;
    }

    void inspectorImGui() {
        ImGui::Text("t: %f", prevT);
        ImGui::DragFloat("Duration", &duration, 0.1f, 0.01);
        if (ImGui::Button("Restart")) {
            prevT = 0;
            playing = true;
        }
        if (ImGui::Button("Play/Pause")) {
            playing = !playing;
        }
    }
};

struct BezierAnimation {

    BezierAnimation() = default;

    BezierAnimation(Anim anim, CubicBezier curve) :
            anim(anim), curve(curve) {}

    Anim anim;
    CubicBezier curve;
};

struct RotationAnimation {

    RotationAnimation() = default;

    RotationAnimation(Anim anim, std::function<glm::vec3(float)> fun) :
            anim(anim), rotationFun(std::move(fun)) {}

    std::function<glm::vec3(float)> rotationFun;
    Anim anim;
};

struct SkinnedMeshAnimationComponent {
    std::vector<std::filesystem::path> paths;
    int currentFrame = 0;
    int totalFrames;

    SkinnedMeshAnimationComponent(std::vector<std::filesystem::path> paths) : paths(paths) {
        totalFrames = paths.size();
    };
};

struct TransformAnimation {

    TransformAnimation() = default;

    TransformAnimation(Anim anim, std::function<Transform(Transform, float)> fun) :
            anim(anim), callback(std::move(fun)) {}

    std::function<Transform(Transform, float)> callback;
    Anim anim;
};
