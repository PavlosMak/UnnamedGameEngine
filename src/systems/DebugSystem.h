#pragma once

#include "framework/entt_imgui/imgui_entt_entity_editor.hpp"
#include "../scene/Components.h"

namespace MM {

    // Define the "inspectors" of each component here

    template<>
    void ComponentEditorWidget<WasdComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &w = reg.get<WasdComponent>(e);
        ImGui::DragFloat("Movement Speed", &w.movementSpeed, 0.001f);
    }

    template<>
    void ComponentEditorWidget<TagComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &t = reg.get<TagComponent>(e);
        ImGui::Text(t.name.c_str());
    }

    template<>
    void ComponentEditorWidget<TransformComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &t = reg.get<TransformComponent>(e);

        ImGui::DragFloat3("Position", &t.localTransform.pos[0], 0.01f);
        ImGui::DragFloat3("Rotation", &t.localTransform.rotation[0], 1.0f);
        ImGui::DragFloat3("Scale", &t.localTransform.scale[0], 0.005f);
    }

    template<>
    void ComponentEditorWidget<CameraComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &cam = reg.get<CameraComponent>(e);
        ImGui::DragFloat("Fov", &cam.camera->fov, 1, 0.1, 175);
    }

    template<>
    void ComponentEditorWidget<LightComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &light = reg.get<LightComponent>(e);
        glm::vec3 newColor = light.light.getColor();
        ImGui::DragFloat3("Color", &newColor[0], 0.001f, 0.0);
        light.light.setColor(newColor);
    }

    template<>
    void ComponentEditorWidget<MaterialComponent>(entt::registry &reg, entt::registry::entity_type e) {
        auto &material = reg.get<MaterialComponent>(e).material;
        ImGui::Text("Material");
        glm::vec3 baseColor = material->getColor();
        float roughness = material->getRoughness();
        float metallic = material->getMetallic();
        float ambient = material->getAmbient();
        ImGui::ColorPicker3("Albedo", &baseColor[0], 0.001f);
        ImGui::DragFloat("Roughness", &roughness, 0.001f, 0.0);
        ImGui::DragFloat("Metallic", &metallic, 0.001f, 0.0);
        ImGui::DragFloat("Ambient", &ambient, 0.001f, 0.0);
        material->setColor(baseColor);
        material->setMetallic(metallic);
        material->setRoughness(roughness);
        material->setAmbient(ambient);
    }
}

class DebugSystem {
private:

    using Editor = MM::EntityEditor<entt::entity>;
    Editor m_editor;

    bool isDisplayed = true;

    std::set<Editor::ComponentTypeID> m_components_filter;
    entt::entity m_current_entity;

public:
    template<typename Component>

    void register_component(const std::string &display_name) {
        m_editor.registerComponent<Component>(display_name);
    }

    void run(entt::registry &registry) {

        if (isDisplayed) {
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
    }

    void onKeyRelease(int key) {
        switch (key) {
            case GLFW_KEY_BACKSPACE:
                this->isDisplayed = !this->isDisplayed;
                break;
        }
    }

};
