#pragma once

#include <utility>
#include "iostream"

#include "glm/vec3.hpp"

class CubicBezier {
public:
    glm::vec3 point1;
    glm::vec3 handle1;
    glm::vec3 handle2;
    glm::vec3 point2;

    CubicBezier() = default;

    CubicBezier(glm::vec3 p1, glm::vec3 h1, glm::vec3 h2 , glm::vec3 p2)
            : point1(p1), handle1(h1), point2(p2), handle2(h2) {}

    glm::vec3 eval(float t) const {
        float tm = 1 - t;
        return tm * tm * tm * point1 + 3 * tm * tm * t * handle1 + 3 * tm * t * t * handle2 + t * t * t * point2;
    }
};

class CompositeBezier {
public:
    std::vector<CubicBezier> curves;
    int nCurves;
    float step;

    CompositeBezier() = default;

    CompositeBezier(std::vector<glm::vec3> points) {
        this->nCurves = (int) (points.size() - 1) / 3;
        this->step = 1.0f / (float) this->nCurves;

        for (auto p : points) {
            std::cout << p.x << " " << p.y << " " << p.z << std::endl;
        }
        std::cout << std::endl;

        for (int i=0; i<nCurves; i++) {
            int offset = i * 3;
            auto p1 = points[offset];
            auto h1 = points[offset+1];
            auto h2 = points[offset+2];
            auto p2 = points[offset+3];

            auto curve = CubicBezier(p1, h1, h2, p2);

            std::cout << p1.x << " " << p1.y << " " << p1.z << std::endl;
            std::cout << h1.x << " " << h1.y << " " << h1.z << std::endl;
            std::cout << h2.x << " " << h2.y << " " << h2.z << std::endl;
            std::cout << p2.x << " " << p2.y << " " << p2.z << std::endl;
            std::cout << std::endl;

            this->curves.emplace_back(curve);

        }
    }

    glm::vec3 eval(float t) {
        int curveI = std::min((int) std::floor(t / this->step), nCurves - 1);
        float curveLo = (float) curveI * this->step;
        float curveHi = (float) (curveI+1) * this->step;
        float tMapped = (t - curveLo) / (curveHi - curveLo);

        return curves[curveI].eval(tMapped);
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

    BezierAnimation(Anim anim, CompositeBezier curve) :
            anim(anim), curve(std::move(curve)) {}

    Anim anim;
    CompositeBezier curve;
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

    SkinnedMeshAnimationComponent() = default;

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
