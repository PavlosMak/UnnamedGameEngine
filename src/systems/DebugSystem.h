#pragma once

#include "framework/entt_imgui/imgui_entt_entity_editor.hpp"
#include "../scene/Components.h"

namespace MM {

    // Define the "inspectors" of each component here

    template<>
    void ComponentEditorWidget<WasdComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &w = reg.get<WasdComponent>(e);
        ImGui::DragFloat("Movement Speed", &w.movementSpeed, 0.1f);
    }

    template<>
    void ComponentEditorWidget<TagComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &t = reg.get<TagComponent>(e);
        ImGui::Text(t.name.c_str());
    }

    template<>
    void ComponentEditorWidget<TransformComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &t = reg.get<TransformComponent>(e);

        ImGui::DragFloat3("Position", &t.localTransform[3][0], 0.001f);
    }

    template<>
    void ComponentEditorWidget<SRTTransformComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &t = reg.get<SRTTransformComponent>(e);

        ImGui::DragFloat3("Position", &t.pos[0], 0.01f);
        ImGui::DragFloat3("Rotation", &t.rotation[0], 1.0f);
        ImGui::DragFloat3("Scale", &t.scale[0], 0.005f);
    }

    template<>
    void ComponentEditorWidget<CameraComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &cam = reg.get<CameraComponent>(e);

        ImGui::DragFloat("Fov", &cam.fov, 0.001f);
        ImGui::DragFloat3("LookTarget", &cam.lookTarget[0], 0.001f);
    }
}

class DebugSystem {
private:
    using Editor = MM::EntityEditor<entt::entity>;
    Editor m_editor;

    std::set<Editor::ComponentTypeID> m_components_filter;
    entt::entity m_current_entity;

public:
    template<typename Component>
    void register_component(const std::string &display_name) {
        m_editor.registerComponent<Component>(display_name);
    }

    virtual void run(entt::registry &registry) {

        ImGui::Begin("Inspector");

        ImGui::Text("Application average: %.3f ms/frame (%.3f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::Separator();

        ImGui::Columns(2);
        ImGui::BeginChild("Entities");
        m_editor.renderEntityList(registry, m_components_filter);
        ImGui::EndChild();
        ImGui::NextColumn();

        ImGui::BeginChild("Entity Editor");
        m_editor.renderEditor(registry, m_current_entity);
        ImGui::EndChild();
        ImGui::NextColumn();

        ImGui::End();
    }
};
