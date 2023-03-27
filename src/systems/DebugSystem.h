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

        ImGui::DragFloat3("Position", &t.transform[3][0], 0.001f);
    }

    template<>
    void ComponentEditorWidget<CameraComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &cam = reg.get<CameraComponent>(e);

        ImGui::DragFloat("Fov", &cam.fov, 0.001f);
        ImGui::DragFloat3("LookTarget", &cam.lookTarget[0], 0.001f);
    }

    template<>
    void ComponentEditorWidget<LightComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &light = reg.get<LightComponent>(e);
        glm::vec3 newColor = light.light.getColor();
        ImGui::DragFloat3("Color", &newColor[0],0.001f,0.0);
        light.light.setColor(newColor);
    }

    template<>
    void ComponentEditorWidget<MaterialComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &material = reg.get<MaterialComponent>(e).material;
        ImGui::Text("Material");
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