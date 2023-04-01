#pragma once

#include "framework/entt_imgui/imgui_entt_entity_editor.hpp"

struct SetRotation {
    SetRotation() = default;
    SetRotation(float rotation, int axis) : rotation(rotation), axis(axis) {};
    float rotation;
    int axis;
};

struct RobotClaw1 {
    RobotClaw1() = default;
    float rotation;
};

struct RobotClaw2 {
    RobotClaw2() = default;
    float rotation;
};

